//
// Created by reedt on 9/25/2019.
//

#include <ids.h>
#include <stdio.h>
#include "d_expr.h"

d_expr * create_d_expr(GArray * token_stream, unsigned long index, unsigned long * next)
{
    d_expr * new_d_expr = calloc(1, sizeof(d_expr));

    // This could be:
    // 1. ID (Double type)
    // 2. f_call (Double return)
    // 3. double (literal)
    // 4. i_expr op i_expr

    Token * curToken = &g_array_index(token_stream, Token, index);
    Token * nextToken = &g_array_index(token_stream, Token, index+1);
    Type check;

    if(nextToken->type == t_end_paren || nextToken->type == t_end_stmt || nextToken->type == t_comma)
    {
        // The next token is not part of the expression, so it is a singular value

        *next = index+1;
        if(curToken->type == t_id)
        {
            if(findIDType(curToken->data, &check) && check == jdouble)
            {
                // We have an already declared double variable
                new_d_expr->id = curToken;
            }
            else
            {
                // Its an ID, but it either hasn't been declared or is not an double
                fprintf(stderr, "Syntax Error: Tried to use a variable that has not been declared or is not an double");
                exit(-1);
            }
        }
        else if(curToken->type == t_floating)
        {
            new_d_expr->double_literal = curToken;
        }
        else
        {
            // We have a single token d_expression that isn't an ID or literal, who knows?
            fprintf(stderr, "Syntax Error: Unexpected Token when building double expression");
            exit(-1);
        }
    }
    else if(curToken->type == t_plus || curToken->type == t_minus)
    {
        if(nextToken->type == t_floating)
        {
            *next = index+1;
            // This is a 'sign' symbol, not an operation
            Token * thirdToken = &g_array_index(token_stream, Token, index+2);
            if(thirdToken->type == t_end_paren || thirdToken->type == t_end_stmt || thirdToken->type == t_comma)
            {
                // Expression is just sign and literal
                new_d_expr->operator = curToken;
                (*next)++;
                new_d_expr->double_literal = nextToken;
            }
            else
            {
                // More complex expression ahead
                new_d_expr->LHS_expr = calloc(1, sizeof(d_expr));
                new_d_expr->LHS_expr->operator = curToken;
                new_d_expr->LHS_expr->double_literal = nextToken;

                switch (thirdToken->type)
                {
                    case t_plus:
                    case t_multiply:
                    case t_divide:
                    case t_minus:
                    case t_power:
                        new_d_expr->operator = thirdToken;
                        break;
                    default:
                        // Next token is not an operation
                        fprintf(stderr, "Syntax Error: Expected an double operator");
                        exit(-1);
                }

                new_d_expr->RHS_expr = create_d_expr(token_stream, index + 3, next);

            }
        }
        else
        {
            // We have a 'sign' symbol, but not a Double literal after
            fprintf(stderr, "Syntax Error: Expected a double literal following sign");
            exit(-1);
        }
    }
    else
    {
        // We know we have more than one token in the expression now
        new_d_expr->LHS_expr = calloc(1, sizeof(d_expr));

        if(curToken->type == t_floating)
        {
            new_d_expr->LHS_expr->double_literal = curToken;
        }
        else if(curToken->type == t_id)
        {
            if(findIDType(curToken->data, &check) && check == jdouble)
            {
                // We have an already declared double variable
                new_d_expr->LHS_expr->id = curToken;
            }
            else
            {
                // Its an ID, but it either hasn't been declared or is not an double
                fprintf(stderr, "Syntax Error: Tried to use a variable that has not been declared or is not a double");
                exit(-1);
            }
        }

        switch (nextToken->type)
        {
            case t_plus:
            case t_multiply:
            case t_divide:
            case t_minus:
            case t_power:
                new_d_expr->operator = nextToken;
                break;
            default:
                // Next token is not an operation
                fprintf(stderr, "Syntax Error: Expected an double operator");
                exit(-1);
        }
        new_d_expr->RHS_expr = create_d_expr(token_stream, index + 2, next);
    }

    return new_d_expr;
}

GString * d_expr_to_json(d_expr * dExpr)
{
    GString * retVal = g_string_new(NULL);
    if(dExpr != NULL)
    {
        g_string_append(retVal, "{\"Double_Function_Call\": ");
        if(dExpr->function_call != NULL)
        {
            GString * child = f_call_to_json(dExpr->function_call);
            g_string_append(retVal, child->str);
            g_string_free(child, TRUE);
        }
        else
        {
            g_string_append(retVal, "null");
        }

        g_string_append(retVal, ", \"Double_Literal\": ");
        if(dExpr->double_literal != NULL)
        {
            GString * child = token_to_json(dExpr->double_literal);
            g_string_append(retVal, child->str);
            g_string_free(child, TRUE);
        }
        else
        {
            g_string_append(retVal, "null");
        }

        g_string_append(retVal, ", \"ID_Double\": ");
        if(dExpr->id != NULL)
        {
            GString * child = token_to_json(dExpr->id);
            g_string_append(retVal, child->str);
            g_string_free(child, TRUE);
        }
        else
        {
            g_string_append(retVal, "null");
        }

        g_string_append(retVal, ", \"LHS_Double_Expression\": ");
        if(dExpr->LHS_expr != NULL)
        {
            GString * child = d_expr_to_json(dExpr->LHS_expr);
            g_string_append(retVal, child->str);
            g_string_free(child, TRUE);
        }
        else
        {
            g_string_append(retVal, "null");
        }

        g_string_append(retVal, ", \"Operator\": ");
        if(dExpr->operator != NULL)
        {
            GString * child = token_to_json(dExpr->operator);
            g_string_append(retVal, child->str);
            g_string_free(child, TRUE);
        }
        else
        {
            g_string_append(retVal, "null");
        }

        g_string_append(retVal, ", \"RHS_Double_Expression\": ");
        if(dExpr->RHS_expr != NULL)
        {
            GString * child = d_expr_to_json(dExpr->RHS_expr);
            g_string_append(retVal, child->str);
            g_string_free(child, TRUE);
        }
        else
        {
            g_string_append(retVal, "null");
        }

        g_string_append_c(retVal, '}');
    }
    else
    {
        g_string_append(retVal, "null");
    }
    return retVal;
}

void destroy_d_expr(d_expr * dExpr)
{
    if(dExpr != NULL)
    {
        if(dExpr->function_call != NULL)
        {
            destroy_f_call(dExpr->function_call);
        }
        if(dExpr->LHS_expr != NULL)
        {
            destroy_d_expr(dExpr->LHS_expr);
        }
        if(dExpr->RHS_expr != NULL)
        {
            destroy_d_expr(dExpr->RHS_expr);
        }
        free(dExpr);
    }
}

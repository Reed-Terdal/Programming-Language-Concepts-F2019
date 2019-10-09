//
// Created by reedt on 9/25/2019.
//

#include <stdio.h>
#include "i_expr.h"
#include "ids.h"

i_expr * create_i_expr(GArray * token_stream, unsigned long index, unsigned long * next)
{
    i_expr * new_i_expr = calloc(1, sizeof(i_expr));

    // This could be:
    // 1. ID (Integer type)
    // 2. f_call (Integer return)
    // 3. int (literal)
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
            if(findIDType(curToken->data, &check) && check == jint)
            {
                // We have an already declared int variable
                new_i_expr->id = curToken;
            }
            else
            {
                // Its an ID, but it either hasn't been declared or is not an int
                fprintf(stderr, "Syntax Error: Tried to use a variable that has not been declared or is not an int");
                exit(-1);
            }
        }
        else if(curToken->type == t_integer)
        {
            new_i_expr->int_literal = curToken;
        }
        else
        {
            // We have a single token i_expression that isn't an ID or literal, who knows?
            fprintf(stderr, "Syntax Error: Unexpected Token when building integer expression");
            exit(-1);
        }
    }
    else if(curToken->type == t_plus || curToken->type == t_minus)
    {
        if(nextToken->type == t_integer)
        {
            *next = index+1;
            // This is a 'sign' symbol, not an operation
            Token * thirdToken = &g_array_index(token_stream, Token, index+2);
            if(thirdToken->type == t_end_paren || thirdToken->type == t_end_stmt || thirdToken->type == t_comma)
            {
                // Expression is just sign and literal
                new_i_expr->operator = curToken;
                (*next)++;
                new_i_expr->int_literal = nextToken;
            }
            else
            {
                // More complex expression ahead
                new_i_expr->LHS_expr = calloc(1, sizeof(i_expr));
                new_i_expr->LHS_expr->operator = curToken;
                new_i_expr->LHS_expr->int_literal = nextToken;

                switch (thirdToken->type)
                {
                    case t_plus:
                    case t_multiply:
                    case t_divide:
                    case t_minus:
                    case t_power:
                        new_i_expr->operator = thirdToken;
                        break;
                    default:
                        // Next token is not an operation
                        fprintf(stderr, "Syntax Error: Expected an integer operator");
                        exit(-1);
                }

                new_i_expr->RHS_expr = create_i_expr(token_stream, index+3, next);

            }
        }
        else
        {
            // We have a 'sign' symbol, but not a Integer literal after
            fprintf(stderr, "Syntax Error: Expected a integer literal following sign");
            exit(-1);
        }
    }
    else
    {
        // We know we have more than one token in the expression now
        new_i_expr->LHS_expr = calloc(1, sizeof(i_expr));

        if(curToken->type == t_integer)
        {
            new_i_expr->LHS_expr->int_literal = curToken;
        }
        else if(curToken->type == t_id)
        {
            if(findIDType(curToken->data, &check) && check == jint)
            {
                // We have an already declared int variable
                new_i_expr->LHS_expr->id = curToken;
            }
            else
            {
                // Its an ID, but it either hasn't been declared or is not an int
                fprintf(stderr, "Syntax Error: Tried to use a variable that has not been declared or is not an int");
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
                new_i_expr->operator = nextToken;
                break;
            default:
                // Next token is not an operation
                fprintf(stderr, "Syntax Error: Expected an integer operator");
                exit(-1);
        }
        new_i_expr->RHS_expr = create_i_expr(token_stream, index+2, next);
    }

    return new_i_expr;
}

GString * i_expr_to_json(i_expr * iExpr)
{
    GString * retVal = g_string_new(NULL);
    if(iExpr != NULL)
    {
        g_string_append(retVal, "{\"Int_Function_Call\": ");
        if(iExpr->function_call != NULL)
        {
            GString * child = f_call_to_json(iExpr->function_call);
            g_string_append(retVal, child->str);
            g_string_free(child, TRUE);
        }
        else
        {
            g_string_append(retVal, "null");
        }

        g_string_append(retVal, ", \"Int_Literal\": ");
        if(iExpr->int_literal != NULL)
        {
            GString * child = token_to_json(iExpr->int_literal);
            g_string_append(retVal, child->str);
            g_string_free(child, TRUE);
        }
        else
        {
            g_string_append(retVal, "null");
        }

        g_string_append(retVal, ", \"ID_Int\": ");
        if(iExpr->id != NULL)
        {
            GString * child = token_to_json(iExpr->id);
            g_string_append(retVal, child->str);
            g_string_free(child, TRUE);
        }
        else
        {
            g_string_append(retVal, "null");
        }

        g_string_append(retVal, ", \"LHS_Int_Expression\": ");
        if(iExpr->LHS_expr != NULL)
        {
            GString * child = i_expr_to_json(iExpr->LHS_expr);
            g_string_append(retVal, child->str);
            g_string_free(child, TRUE);
        }
        else
        {
            g_string_append(retVal, "null");
        }

        g_string_append(retVal, ", \"Operator\": ");
        if(iExpr->operator != NULL)
        {
            GString * child = token_to_json(iExpr->operator);
            g_string_append(retVal, child->str);
            g_string_free(child, TRUE);
        }
        else
        {
            g_string_append(retVal, "null");
        }

        g_string_append(retVal, ", \"RHS_Int_Expression\": ");
        if(iExpr->RHS_expr != NULL)
        {
            GString * child = i_expr_to_json(iExpr->RHS_expr);
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

void destroy_i_expr(i_expr * iExpr)
{
    if(iExpr != NULL)
    {
        if(iExpr->function_call != NULL)
        {
            destroy_f_call(iExpr->function_call);
        }
        if(iExpr->LHS_expr != NULL)
        {
            destroy_i_expr(iExpr->LHS_expr);
        }
        if(iExpr->RHS_expr != NULL)
        {
            destroy_i_expr(iExpr->RHS_expr);
        }
        free(iExpr);
    }
}

//
// Created by reedt on 9/25/2019.
//

#include <stdio.h>
#include "i_expr.h"
#include "ids.h"

i_expr * internal_i_expr_constructor(i_expr * parent, GArray * token_stream, unsigned long index, unsigned long * next);

i_expr * create_i_expr(GArray * token_stream, unsigned long index, unsigned long * next)
{
    return internal_i_expr_constructor(NULL, token_stream, index, next);
}

i_expr * internal_i_expr_constructor(i_expr * parent, GArray * token_stream, unsigned long index, unsigned long * next)
{
    i_expr * new_i_expr = calloc(1, sizeof(i_expr));
    i_expr * ret_val = NULL;
    // This could be:
    // 1. ID (Integer type)
    // 2. f_call (Integer return)
    // 3. int (literal)
    // 4. i_expr op i_expr

    Token * curToken = &g_array_index(token_stream, Token, index);
    Token * nextToken = &g_array_index(token_stream, Token, index+1);
    Type check;

    unsigned long curIndex = index;
    /// Start of LHS
    if(parent == NULL)
    {
        // This is the entrance of our recursive builder
        new_i_expr->LHS_expr = calloc(1, sizeof(i_expr));
        if(curToken->type == t_plus || curToken->type == t_minus)
        {
            // We have a sign symbol
            if(nextToken->type == t_integer)
            {
                new_i_expr->LHS_expr->literal = create_int_node(nextToken, curToken);
                curIndex += 2;
                curToken = &g_array_index(token_stream, Token, curIndex);
            }
        }
        else
        {
            if(curToken->type == t_integer)
            {
                new_i_expr->LHS_expr->literal = create_int_node(curToken, NULL);
            }
            else if(curToken->type == t_id)
            {
                if(findIDType(curToken->data, &check) && check == jint)
                {
                    // We have an already declared int variable
                    new_i_expr->LHS_expr->id = create_id_node(curToken);
                }
                else
                {
                    // Its an ID, but it either hasn't been declared or is not an int
                    fprintf(stderr, "Syntax Error: Tried to use a variable that has not been declared or is not an int");
                    exit(-1);
                }
            }
            else
            {
                fprintf(stderr, "Syntax Error: Unexpected Token type in int expression");
                exit(-1);
            }
            curIndex++;
            curToken = &g_array_index(token_stream, Token, curIndex);
        }
    }
    else
    {
        // This is a recursive call
        // we already have a LHS
        new_i_expr->LHS_expr = parent;
    }
    /// END OF LHS

    /// START OF OPERATOR
    switch (curToken->type)
    {
        case t_plus:
        case t_multiply:
        case t_divide:
        case t_minus:
        case t_power:
            new_i_expr->operatorNode = create_operator(curToken);
            break;
        case t_end_paren:
        case t_comma:
        case t_end_stmt:
            // We are done, there is only a LHS, so transfer it to be "this" expression
            (*next) = curIndex;
            ret_val = new_i_expr->LHS_expr;
            free(new_i_expr); // We don't want it anymore, because it only has an LHS node
            break;
        default:
            // Next token is not an operation
            fprintf(stderr, "Syntax Error: Expected an integer operator");
            exit(-1);
    }
    /// END OF OPERATOR
    /// START OF RHS
    if(ret_val == NULL)
    {
        // There is more to go
        curIndex++;
        curToken = &g_array_index(token_stream, Token, curIndex);
        nextToken = &g_array_index(token_stream, Token, curIndex + 1);

        new_i_expr->RHS_expr = calloc(1, sizeof(i_expr));
        if (curToken->type == t_plus || curToken->type == t_minus)
        {
            // We have a sign symbol
            if (nextToken->type == t_integer)
            {
                new_i_expr->RHS_expr->literal = create_int_node(nextToken, curToken);
                curIndex += 2;
                curToken = &g_array_index(token_stream, Token, curIndex);
            }
        }
        else
        {
            if (curToken->type == t_integer)
            {
                new_i_expr->RHS_expr->literal = create_int_node(curToken, NULL);
            }
            else if (curToken->type == t_id)
            {
                if (findIDType(curToken->data, &check) && check == jint)
                {
                    // We have an already declared int variable
                    new_i_expr->RHS_expr->id = create_id_node(curToken);
                }
                else
                {
                    // Its an ID, but it either hasn't been declared or is not an int
                    fprintf(stderr, "Syntax Error: Tried to use a variable that has not been declared or is not an int");
                    exit(-1);
                }
            }
            else
            {
                fprintf(stderr, "Syntax Error: Unexpected Token type in int expression");
                exit(-1);
            }
            curIndex++;
            curToken = &g_array_index(token_stream, Token, curIndex);
        }
        if (curToken->type == t_end_paren || curToken->type == t_end_stmt || curToken->type == t_comma)
        {
            // We are done, return this expression
            (*next) = curIndex;
            ret_val = new_i_expr;
        }
        else
        {
            // More to go, need to recurse
            ret_val = internal_i_expr_constructor(new_i_expr, token_stream, curIndex, next);
        }
    }
    /// END OF RHS
    return ret_val;
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
        if(iExpr->literal != NULL)
        {
            GString * child = int_node_to_json(iExpr->literal);
            g_string_append(retVal, child->str);
            g_string_free(child, TRUE);
        }
        else
        {
            g_string_append(retVal, "null");
        }

        g_string_append(retVal, ", \"ID_Node\": ");
        if(iExpr->id != NULL)
        {
            GString * child = id_node_to_json(iExpr->id);
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
        if(iExpr->operatorNode != NULL)
        {
            GString * child = operator_to_json(iExpr->operatorNode);
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
        if(iExpr->literal != NULL)
        {
            destroy_int_node(iExpr->literal);
        }
        if(iExpr->operatorNode != NULL)
        {
            destroy_operator(iExpr->operatorNode);
        }
        if(iExpr->id != NULL)
        {
            destroy_id_node(iExpr->id);
        }
        free(iExpr);
    }
}

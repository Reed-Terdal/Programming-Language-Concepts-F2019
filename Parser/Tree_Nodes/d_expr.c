/**************************************************************************************************
*
* File Name: d_expr_node.c
*
* Author: Reed Terdal
*
* Created for CS344 on: 09/25/2019
*
* Purpose: code that provides access to functions for manipulating double expression nodes.
*
**************************************************************************************************/

#include <ids.h>
#include <stdio.h>
#include "d_expr.h"

d_expr * internal_d_expr_constructor(d_expr * parent, GArray * token_stream, unsigned long index, unsigned long * next);

d_expr * create_d_expr(GArray * token_stream, unsigned long index, unsigned long * next)
{
    return internal_d_expr_constructor(NULL, token_stream, index, next);
}

d_expr * internal_d_expr_constructor(d_expr * parent, GArray * token_stream, unsigned long index, unsigned long * next)
{
    d_expr * new_d_expr = calloc(1, sizeof(d_expr));
    d_expr * ret_val = NULL;
    // This could be:
    // 1. ID (double type)
    // 2. f_call (double return)
    // 3. double (literal)
    // 4. d_expr op d_expr

    Token * curToken = &g_array_index(token_stream, Token, index);
    Token * nextToken = &g_array_index(token_stream, Token, index+1);
    Type check;

    unsigned long curIndex = index;
    /// Start of LHS
    if(parent == NULL)
    {
        // This is the entrance of our recursive builder
        new_d_expr->LHS_expr = calloc(1, sizeof(d_expr));
        if(curToken->type == t_plus || curToken->type == t_minus)
        {
            // We have a sign symbol
            if(nextToken->type == t_floating)
            {
                new_d_expr->LHS_expr->double_literal = create_double_node(nextToken, curToken);
                curIndex += 2;
                curToken = &g_array_index(token_stream, Token, curIndex);
            }
        }
        else
        {
            if(curToken->type == t_floating)
            {
                new_d_expr->LHS_expr->double_literal = create_double_node(curToken, NULL);
            }
            else if(curToken->type == t_id)
            {
                if(findIDType(curToken->data, &check))
                {
                    if(check == jdouble)
                    {
                        // We have an already declared double variable
                        new_d_expr->LHS_expr->id = create_id_node(curToken);
                    }
                    else
                    {
                        // The ID is not an Double
                        type_error(g_string_new("(Double Literal, Double Function)"), check, token_stream, curIndex);
                    }
                }
                else
                {
                    // Its an ID, but it either hasn't been declared
                    undeclared_error(curToken->data, token_stream, curIndex);
                }
            }
            else
            {
                unexpected_token_error(g_string_new("(Double Literal, Double ID, Double Function)"), curToken->type,
                                       token_stream, curIndex);
            }
            curIndex++;
            curToken = &g_array_index(token_stream, Token, curIndex);
        }
    }
    else
    {
        // This is a recursive call
        // we already have a LHS
        new_d_expr->LHS_expr = parent;
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
            new_d_expr->operator = create_operator(curToken);
            break;
        case t_end_paren:
        case t_comma:
        case t_end_stmt:
        case t_comp_greater:    // Comparisons are handled in i_expr, so we need to backtrack
        case t_comp_goe:        // Comparisons are handled in i_expr, so we need to backtrack
        case t_comp_eq:         // Comparisons are handled in i_expr, so we need to backtrack
        case t_comp_neq:        // Comparisons are handled in i_expr, so we need to backtrack
        case t_comp_loe:        // Comparisons are handled in i_expr, so we need to backtrack
        case t_comp_less:       // Comparisons are handled in i_expr, so we need to backtrack
            // We are done, there is only a LHS, so transfer it to be "this" expression
            (*next) = curIndex;
            ret_val = new_d_expr->LHS_expr;
            free(new_d_expr); // We don't want it anymore, because it only has an LHS node
            break;
        default:
            // Next token is not an operation
            unexpected_token_error(g_string_new("(+ - * / ^)"), curToken->type, token_stream, curIndex);
    }
    /// END OF OPERATOR
    /// START OF RHS
    if(ret_val == NULL)
    {
        // There is more to go
        curIndex++;
        curToken = &g_array_index(token_stream, Token, curIndex);
        nextToken = &g_array_index(token_stream, Token, curIndex + 1);

        new_d_expr->RHS_expr = calloc(1, sizeof(d_expr));
        if (curToken->type == t_plus || curToken->type == t_minus)
        {
            // We have a sign symbol
            if (nextToken->type == t_floating)
            {
                new_d_expr->RHS_expr->double_literal = create_double_node(nextToken, curToken);
                curIndex += 2;
                curToken = &g_array_index(token_stream, Token, curIndex);
            }
        }
        else
        {
            if (curToken->type == t_floating)
            {
                new_d_expr->RHS_expr->double_literal = create_double_node(curToken, NULL);
            }
            else if (curToken->type == t_id)
            {
                if (findIDType(curToken->data, &check))
                {
                    if(check == jdouble)
                    {
                        // We have an already declared double variable
                        new_d_expr->RHS_expr->id = create_id_node(curToken);
                    }
                    else
                    {
                        // The ID is not an Double
                        type_error(g_string_new("(Double Literal, Double Function)"), check, token_stream, curIndex);
                    }
                }
                else
                {
                    // Its an ID, but it either hasn't been declared
                    undeclared_error(curToken->data, token_stream, curIndex);
                }
            }
            else
            {
                unexpected_token_error(g_string_new("(Double Literal, Double ID, Double Function)"), curToken->type,
                                       token_stream, curIndex);
            }
            curIndex++;
            curToken = &g_array_index(token_stream, Token, curIndex);
        }
        switch (curToken->type)
        {
            case t_end_paren:
            case t_end_stmt:
            case t_comma:
            case t_comp_less:
            case t_comp_loe:
            case t_comp_eq:
            case t_comp_neq:
            case t_comp_goe:
            case t_comp_greater:
                // We are done
                (*next) = curIndex;
                ret_val = new_d_expr;
                break;
            default:
                // Recurse more
                ret_val = internal_d_expr_constructor(new_d_expr, token_stream, curIndex, next);
        }
    }
    /// END OF RHS
    return ret_val;
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
            GString * child = double_node_to_json(dExpr->double_literal);
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
            GString * child = id_node_to_json(dExpr->id);
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
            GString * child = operator_to_json(dExpr->operator);
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
        if(dExpr->double_literal != NULL)
        {
            destroy_double_node(dExpr->double_literal);
        }
        if(dExpr->operator != NULL)
        {
            destroy_operator(dExpr->operator);
        }
        if(dExpr->id != NULL)
        {
            destroy_id_node(dExpr->id);
        }
        free(dExpr);
    }
}

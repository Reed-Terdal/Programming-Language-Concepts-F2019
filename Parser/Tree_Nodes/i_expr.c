/**************************************************************************************************
*
* File Name: i_expr.c
*
* Author: Reed Terdal
*
* Created for CS344 on: 09/25/2019
*
* Purpose: code that provides functions for manipulating integer expression nodes.
*
**************************************************************************************************/


#include <stdio.h>
#include "i_expr.h"
#include "ids.h"
#include "Errors.h"

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

        bool isInteger = false;

        new_i_expr->LHS_expr = calloc(1, sizeof(expr));
        switch (curToken->type)
        {
            case t_plus:
            case t_minus:
                // We have a sign token, should be followed by int or double literal
                if(nextToken->type == t_floating)
                {
                    // Floating, make the LHS expression double
                    new_i_expr->LHS_expr->double_expression = create_d_expr(token_stream, index, next);
                }
                else if(nextToken->type == t_integer)
                {
                    isInteger = true;
                }
                else
                {
                    fprintf(stderr, "Unexpected token following a sign token\n");
                    exit(-1);
                }
                break;
            case t_integer:
                isInteger = true;
                break;
            case t_floating:
                new_i_expr->LHS_expr->double_expression = create_d_expr(token_stream, index, next);
                break;
            case t_string:
                new_i_expr->LHS_expr->string_expression = create_s_expr(token_stream, index, next);
                break;
            case t_id:
            {
                // Need to determine the type of the ID to make a decision
                Type idType;
                if(!findIDType(curToken->data, &idType))
                {
                    fprintf(stderr, "Undeclared variable used in i_expr\n");
                    exit(-1);
                }
                switch (idType)
                {
                    case jf_str:
                    case jstring:
                        new_i_expr->LHS_expr->string_expression = create_s_expr(token_stream, index, next);
                        break;
                    case jf_double:
                    case jdouble:
                        new_i_expr->LHS_expr->double_expression = create_d_expr(token_stream, index, next);
                        break;
                    case jf_int:
                    case jint:
                        isInteger = true;
                        break;
                    default:
                        fprintf(stderr, "Unexpected ID type in i_expr\n");
                        exit(-1);
                }
            }
                break;
            default:
                fprintf(stderr, "Unexpected token when creating i_expr\n");
                exit(-1);
        }

        if(!isInteger)
        {
            curToken = &g_array_index(token_stream, Token, *next);
            switch (curToken->type)
            {
                case t_comp_less:
                case t_comp_loe:
                case t_comp_eq:
                case t_comp_neq:
                case t_comp_goe:
                case t_comp_greater:
                    new_i_expr->operatorNode = create_operator(curToken);
                    (*next)++;
                    break;
                default:
                    fprintf(stderr, "Expected a comparator in i_expr\n");
                    exit(-1);
            }

            new_i_expr->RHS_expr = calloc(1, sizeof(expr));
            // Since we are purely L-R evaluation, the next and ONLY the next token must match the LHS
            curToken = &g_array_index(token_stream, Token, *next);
            switch (curToken->type)
            {
                case t_plus:
                case t_minus:
                    nextToken = &g_array_index(token_stream, Token, *next + 1);
                    if(nextToken->type == t_floating)
                    {
                        new_i_expr->RHS_expr->double_expression = calloc(1, sizeof(d_expr));
                        new_i_expr->RHS_expr->double_expression->double_literal = create_double_node(curToken, nextToken);
                        (*next)+=2;
                    }
                    else
                    {
                        fprintf(stderr, "Unexpected token following sign token in i_expr compare\n");
                        exit(-1);
                    }
                    break;
                case t_floating:
                    new_i_expr->RHS_expr->double_expression = calloc(1, sizeof(d_expr));
                    new_i_expr->RHS_expr->double_expression->double_literal = create_double_node(curToken, NULL);
                    (*next)++;
                    break;
                case t_string:
                    new_i_expr->RHS_expr->string_expression = calloc(1, sizeof(s_expr));
                    new_i_expr->RHS_expr->string_expression->literal = create_string_node(curToken);
                    (*next)++;
                    break;
                case t_id:
                {
                    Type idType;
                    if(!findIDType(curToken->data, &idType))
                    {
                        fprintf(stderr, "Undeclared ID in RHS of comparison\n");
                        exit(-1);
                    }
                    switch (idType)
                    {
                        case jf_str:
                            new_i_expr->RHS_expr->string_expression = calloc(1, sizeof(s_expr));
                            new_i_expr->RHS_expr->string_expression->function_call = create_f_call(token_stream, *next, next);
                            break;
                        case jf_double:
                            new_i_expr->RHS_expr->double_expression = calloc(1, sizeof(d_expr));
                            new_i_expr->RHS_expr->double_expression->function_call = create_f_call(token_stream, *next, next);
                            break;
                        case jdouble:
                            new_i_expr->RHS_expr->double_expression = calloc(1, sizeof(d_expr));
                            new_i_expr->RHS_expr->double_expression->id = create_id_node(curToken);
                            (*next)++;
                            break;
                        case jstring:
                            new_i_expr->RHS_expr->string_expression->id = create_id_node(curToken);
                            (*next)++;
                            break;
                        default:
                            fprintf(stderr, "Unexpected ID type in RHS of comparison\n");
                            exit(-1);
                    }
                }
                    break;
                default:
                    fprintf(stderr, "Unexpected token following comparator in i_expr\n");
                    exit(-1);
            }

            if(new_i_expr->LHS_expr->string_expression != NULL && new_i_expr->RHS_expr->string_expression == NULL)
            {
                fprintf(stderr, "LHS and RHS expression do not match for comparison\n");
                exit(-1);
            }
            if(new_i_expr->LHS_expr->double_expression != NULL && new_i_expr->RHS_expr->double_expression == NULL)
            {
                fprintf(stderr, "LHS and RHS expression do not match for comparison\n");
                exit(-1);
            }

            curToken = &g_array_index(token_stream, Token, *next);
            if (curToken->type == t_end_paren || curToken->type == t_end_stmt || curToken->type == t_comma)
            {
                // We are done, return this expression
                return new_i_expr;
            }
            else
            {
                // More to go, need to recurse
                return internal_i_expr_constructor(new_i_expr, token_stream, *next, next);
            }
        }
        else
        {
            new_i_expr->LHS_expr->int_expression = calloc(1, sizeof(i_expr));
            // This is where we handle the integer case
            if(curToken->type == t_plus || curToken->type == t_minus)
            {
                // We have a sign symbol
                if(nextToken->type == t_integer)
                {
                    new_i_expr->LHS_expr->int_expression->literal = create_int_node(nextToken, curToken);
                    curIndex += 2;
                    curToken = &g_array_index(token_stream, Token, curIndex);
                }
            }
            else
            {
                if(curToken->type == t_integer)
                {
                    new_i_expr->LHS_expr->int_expression->literal = create_int_node(curToken, NULL);
                }
                else if(curToken->type == t_id)
                {
                    if(findIDType(curToken->data, &check))
                    {
                        if( check == jint)
                        {
                            // We have an already declared int variable
                            new_i_expr->LHS_expr->int_expression->id = create_id_node(curToken);
                        }
                        else
                        {
                            // The ID is not an int
                            type_error(g_string_new("(Integer Literal, Integer Function)"), check, token_stream, curIndex);
                        }
                    }
                    else
                    {
                        // Its an ID, but it hasn't been declared
                        undeclared_error(curToken->data, token_stream, index);
                    }
                }
                else
                {
                    unexpected_token_error(g_string_new("(Integer Literal, Integer ID, Integer Function)"), curToken->type,
                                           token_stream, curIndex);
                }
                curIndex++;
                curToken = &g_array_index(token_stream, Token, curIndex);
            }
        }
    }
    else
    {
        // This is a recursive call
        // we already have a LHS
        new_i_expr->LHS_expr = calloc(1, sizeof(expr));
        new_i_expr->LHS_expr->int_expression = parent;
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
        case t_comp_loe:
        case t_comp_less:
        case t_comp_goe:
        case t_comp_eq:
        case t_comp_neq:
        case t_comp_greater:
            new_i_expr->operatorNode = create_operator(curToken);
            break;
        case t_end_paren:
        case t_comma:
        case t_end_stmt:
            // We are done, there is only an LHS, so transfer it to be "this" expression
            (*next) = curIndex;
            ret_val = new_i_expr->LHS_expr->int_expression;
            free(new_i_expr->LHS_expr);
            free(new_i_expr); // We don't want it anymore, because it only has an LHS node
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

        new_i_expr->RHS_expr = calloc(1, sizeof(expr));
        new_i_expr->RHS_expr->int_expression = calloc(1, sizeof(i_expr));
        if (curToken->type == t_plus || curToken->type == t_minus)
        {
            // We have a sign symbol
            if (nextToken->type == t_integer)
            {
                new_i_expr->RHS_expr->int_expression->literal = create_int_node(nextToken, curToken);
                curIndex += 2;
                curToken = &g_array_index(token_stream, Token, curIndex);
            }
        }
        else
        {
            if (curToken->type == t_integer)
            {
                new_i_expr->RHS_expr->int_expression->literal = create_int_node(curToken, NULL);
            }
            else if (curToken->type == t_id)
            {
                if (findIDType(curToken->data, &check))
                {
                    if(check == jint)
                    {
                        // We have an already declared int variable
                        new_i_expr->RHS_expr->int_expression->id = create_id_node(curToken);
                    }
                    else
                    {
                        // Its an ID, but it is not an int
                        type_error(g_string_new("(Integer Literal, Integer Function)"), check, token_stream, curIndex);
                    }
                }
                else
                {
                    // Its an ID, but it hasn't been declared
                    undeclared_error(curToken->data, token_stream, index);
                }
            }
            else
            {
                unexpected_token_error(g_string_new("(Integer Literal, Integer ID, Integer Function)"), curToken->type,
                                       token_stream, curIndex);
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

        g_string_append(retVal, ", \"LHS_Expression\": ");
        if(iExpr->LHS_expr != NULL)
        {
            GString * child = expr_to_json(iExpr->LHS_expr);
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

        g_string_append(retVal, ", \"RHS_Expression\": ");
        if(iExpr->RHS_expr != NULL)
        {
            GString * child = expr_to_json(iExpr->RHS_expr);
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
            destroy_expr(iExpr->LHS_expr);
        }
        if(iExpr->RHS_expr != NULL)
        {
            destroy_expr(iExpr->RHS_expr);
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

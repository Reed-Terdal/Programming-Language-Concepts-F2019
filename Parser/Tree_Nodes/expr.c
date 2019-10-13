/**************************************************************************************************
*
* File Name: expr.c
*
* Author: Reed Terdal
*
* Created for CS344 on: 09/25/2019
*
* Purpose: code that provides functions for manipulating expression nodes.
*
**************************************************************************************************/

#include <stdio.h>
#include "expr.h"

expr * create_expr(GArray * token_stream, unsigned long index, unsigned long * next)
{
    expr * new_expr = calloc(1, sizeof(expr));

    Token * first = &g_array_index(token_stream, Token, index);
    if(first->type == t_minus || first->type == t_plus)
    {
        first = &g_array_index(token_stream, Token, index+1);
    }

    Type exprType;

    switch (first->type)
    {
        case t_id:
            // Expression starts with an ID, check what type it is (and that it exists)
            if(!findIDType(first->data, &exprType))
            {
                // ID has not been declared yet.
                fprintf(stderr, "Syntax Error: Tried to use variable before it has been declared");
                exit(-1);
            }
            break;
        case t_integer:
            exprType = jint;
            break;
        case t_floating:
            exprType = jdouble;
            break;
        case t_string:
            exprType = jstring;
            break;
        default:
            // Unexpected Token when building expression
            fprintf(stderr, "Syntax Error: Unexpected token when building expression");
            exit(-1);
    }

    switch (exprType)
    {
        case jf_int:
        case jint:
        {
            new_expr->int_expression = create_i_expr(token_stream, index, next);
        }
            break;
        case jf_double:
        case jdouble:
        {
            new_expr->double_expression = create_d_expr(token_stream, index, next);
        }
            break;
        case jf_str:
        case jstring:
        {
            new_expr->string_expression = create_s_expr(token_stream, index, next);
        }
            break;
        default:
            // This means we have a INVALID type or void function call as part of an expression.
            fprintf(stderr, "Syntax Error: mismatch types in expression");
            exit(-1);
    }


    return new_expr;
}

GString * expr_to_json(expr * expression)
{
    GString * retVal = g_string_new(NULL);
    if(expression != NULL)
    {
        g_string_append(retVal, "{\"Int_Expression\": ");
        if(expression->int_expression != NULL)
        {
            GString * child = i_expr_to_json(expression->int_expression);
            g_string_append(retVal, child->str);
            g_string_free(child, TRUE);
        }
        else
        {
            g_string_append(retVal, "null");
        }

        g_string_append(retVal, ", \"String_Expression\": ");
        if(expression->string_expression != NULL)
        {
            GString * child = s_expr_to_json(expression->string_expression);
            g_string_append(retVal, child->str);
            g_string_free(child, TRUE);
        }
        else
        {
            g_string_append(retVal, "null");
        }

        g_string_append(retVal, ", \"Double_Expression\": ");
        if(expression->double_expression != NULL)
        {
            GString * child = d_expr_to_json(expression->double_expression);
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

void destroy_expr(expr * expression)
{
    if(expression != NULL)
    {
        if(expression->string_expression != NULL)
        {
            destroy_s_expr(expression->string_expression);
        }
        if(expression->int_expression != NULL)
        {
            destroy_i_expr(expression->int_expression);
        }
        if(expression->double_expression != NULL)
        {
            destroy_d_expr(expression->double_expression);
        }
        free(expression);
    }
}
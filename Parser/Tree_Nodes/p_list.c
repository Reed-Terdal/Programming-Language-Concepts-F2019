//
// Created by reedt on 9/26/2019.
//

#include <stdio.h>
#include <ids.h>
#include "p_list.h"
#include "expr.h"
#include "Errors.h"


p_list * create_p_list(GArray * token_stream, unsigned long index, unsigned long * next)
{
    p_list * new_p_list = calloc(1, sizeof(p_list));

    Token * curToken = &g_array_index(token_stream, Token, index);

    switch (curToken->type)
    {
        case t_id:
        case t_string:
        case t_floating:
        case t_plus:
        case t_minus:
        case t_integer:
            // Process a parameter
            new_p_list->expression = create_expr(token_stream, index, next);
            curToken = &g_array_index(token_stream, Token, *next);
            if(curToken->type == t_comma)
            {
                // More parameters should be present
                (*next)++;
                if(g_array_index(token_stream, Token, *next).type != t_end_paren)
                {
                    // Need to check for a trailing comma
                    new_p_list->next = create_p_list(token_stream, *next, next);
                }
                else
                {
                    // There was a trailing comma
                    trailing_comma_error(token_stream, *next);
                }
            }
            else if(curToken->type != t_end_paren)
            {
                unexpected_token_error(g_string_new("( ',', Expression, ')' )"), curToken->type, token_stream, index);
            }
            break;
        case t_end_paren:
            // No tokens
            break;
        default:
            unexpected_token_error(g_string_new("( ',', Expression, ')' )"), curToken->type, token_stream, index);
    }
    return new_p_list;
}

GString * p_list_to_json(p_list * pList)
{
    GString * retVal = g_string_new(NULL);
    if(pList != NULL)
    {
        g_string_append(retVal, "{\"Expression\": ");
        if(pList->expression != NULL)
        {
            GString * child = expr_to_json(pList->expression);
            g_string_append(retVal, child->str);
            g_string_free(child, TRUE);
        }
        else
        {
            g_string_append(retVal, "null");
        }

        g_string_append(retVal, ", \"Next_Params\": ");
        if(pList->next != NULL)
        {
            GString * child = p_list_to_json(pList->next);
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

void destroy_p_list(p_list * pList)
{
    if(pList != NULL)
    {
        if(pList->expression != NULL)
        {
            destroy_expr(pList->expression);
        }
        if(pList->next != NULL)
        {
            destroy_p_list(pList->next);
        }
        free(pList);
    }
}

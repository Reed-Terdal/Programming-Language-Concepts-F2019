//
// Created by reedt on 9/25/2019.
//

#include <stdio.h>
#include <ids.h>
#include "s_expr.h"
#include "Errors.h"

s_expr * create_s_expr(GArray * token_stream, unsigned long index, unsigned long * next)
{
    s_expr * new_s_expr = calloc(1, sizeof(s_expr));

    Token * curToken = &g_array_index(token_stream, Token, index);

    switch (curToken->type)
    {
        case t_string:
            // Easy, this is a string literal
            new_s_expr->literal = create_string_node(curToken);
            (*next)= index + 1;
            break;
        case t_id:
        {
            // could be a function call or another string variable
            Type id_type;
            if(findIDType(curToken->data, &id_type))
            {
                // ID exists already
                if(id_type == jstring)
                {
                    // The id is for another string var
                    new_s_expr->id = create_id_node(curToken);
                    (*next)= index + 1;
                }
                else if(id_type == jf_str)
                {
                    // the id is for a string function
                    new_s_expr->function_call = create_f_call(token_stream, index, next);
                }
                else
                {
                    type_error(g_string_new("(String Literal, String Function)"),id_type, token_stream, index);
                }
            }
            else
            {
                undeclared_error(curToken->data, token_stream, index);
            }
        }
            break;
        default:
            unexpected_token_error(g_string_new("(String Literal, String Function, String Variable)"), curToken->type, token_stream, index);
    }

    return new_s_expr;
}

GString * s_expr_to_json(s_expr * sExpr)
{
    GString * retVal = g_string_new(NULL);
    if(sExpr != NULL)
    {
        g_string_append(retVal, "{\"String_Function_Call\": ");
        if(sExpr->function_call != NULL)
        {
            GString * child = f_call_to_json(sExpr->function_call);
            g_string_append(retVal, child->str);
            g_string_free(child, TRUE);
        }
        else
        {
            g_string_append(retVal, "null");
        }

        g_string_append(retVal, ", \"String_Literal\": ");
        if(sExpr->literal != NULL)
        {
            GString * child = string_node_to_json(sExpr->literal);
            g_string_append(retVal, child->str);
            g_string_free(child, TRUE);
        }
        else
        {
            g_string_append(retVal, "null");
        }

        g_string_append(retVal, ", \"ID_String\": ");
        if(sExpr->id != NULL)
        {
            GString * child = id_node_to_json(sExpr->id);
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

void destroy_s_expr(s_expr * sExpr)
{
    if(sExpr != NULL)
    {
        if(sExpr->function_call != NULL)
        {
            destroy_f_call(sExpr->function_call);
        }
        if(sExpr->id != NULL)
        {
            destroy_id_node(sExpr->id);
        }
        if(sExpr->literal != NULL)
        {
            destroy_string_node(sExpr->literal);
        }
        free(sExpr);
    }
}

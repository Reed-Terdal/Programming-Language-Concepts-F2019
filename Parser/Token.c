//
// Created by reedt on 10/9/2019.
//
#include "Token.h"

GString * token_to_json(Token * token)
{
    GString * retVal = g_string_new(NULL);
    if(token != NULL)
    {
        if(token->type == t_string)
        {
            g_string_printf(retVal, "{\"Type\": \"%s\", \"Line\": %u, \"Col\": %u, \"Data\": %s}", tokenTypeStrings[token->type], token->line_num, token->col_num, token->data->str);
        }
        else
        {
            g_string_printf(retVal, "{\"Type\": \"%s\", \"Line\": %u, \"Col\": %u, \"Data\": \"%s\"}", tokenTypeStrings[token->type], token->line_num, token->col_num, token->data->str);
        }
    }
    else
    {
        g_string_append(retVal, "null");
    }
    return retVal;
}

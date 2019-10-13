/**************************************************************************************************
*
* File Name: Token.c
*
* Author: Reed Terdal
*
* Created for CS344 on: 09/21/2019
*
* Purpose: code that implements the token to JSON converter.
*
**************************************************************************************************/

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

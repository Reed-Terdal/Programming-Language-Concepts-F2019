//
// Created by reedt on 10/9/2019.
//

#include "string_node.h"


string_node * create_string_node(Token * literal)
{
    string_node * retval = NULL;
    if(literal != NULL && literal->type == t_string)
    {
        retval = calloc(1, sizeof(string_node));
        retval->value = g_string_new(literal->data->str);
        g_string_truncate(retval->value, retval->value->len-1);
        g_string_erase(retval->value, 0, 1);
    }
    return retval;
}

GString * string_node_to_json(string_node * stringNode)
{
    GString * retval = g_string_new(NULL);
    if(stringNode != NULL)
    {
        g_string_append(retval, "{\"Value\": ");
        if(stringNode->value != NULL)
        {
            g_string_append_printf(retval, "\"%s\"}", stringNode->value->str);
        }
        else
        {
            g_string_append(retval, "null}");
        }
    }
    else
    {
        g_string_append(retval, "null");
    }
    return retval;
}

void destroy_string_node(string_node * stringNode)
{
    if(stringNode != NULL)
    {
        if(stringNode->value != NULL)
        {
            g_string_free(stringNode->value, TRUE);
        }
        free(stringNode);
    }
}
/**************************************************************************************************
*
* File Name: id_node.c
*
* Author: Reed Terdal
*
* Created for CS344 on: 10/09/2019
*
* Purpose: Code that provides functions for manipulating ID nodes.
*
**************************************************************************************************/

#include <stdio.h>
#include "id_node.h"


id_node * create_id_node(Token * idToken)
{
    id_node * retVal = NULL;
    if(idToken != NULL && idToken->type == t_id)
    {
        retVal = calloc(1, sizeof(id_node));
        retVal->id = g_string_new(idToken->data->str);
        if(!findIDType(retVal->id, &retVal->type))
        {
            fprintf(stderr, "Syntax Error: Tried to create an ID Node with an undeclared ID");
            exit(-1);
        }
    }
    return retVal;
}

GString * id_node_to_json(id_node * idNode)
{
    GString * retval = g_string_new(NULL);

    if(retval != NULL)
    {
        g_string_printf(retval, "{\"ID\": \"%s\", \"Type\": ", idNode->id->str);
        switch (idNode->type)
        {
            case jf_void:
                g_string_append(retval, "\"VOID Function\"}");
                break;
            case jf_str:
                g_string_append(retval, "\"String Function\"}");
                break;
            case jf_double:
                g_string_append(retval, "\"Double Function\"}");
                break;
            case jf_int:
                g_string_append(retval, "\"Int Function\"}");
                break;
            case jint:
                g_string_append(retval, "\"Int\"}");
                break;
            case jdouble:
                g_string_append(retval, "\"Double\"}");
                break;
            case jstring:
                g_string_append(retval, "\"String\"}");
                break;
            default:
                g_string_append(retval, "null}");
        }
    }
    else
    {
        g_string_append(retval, "null");
    }
    return retval;
}

void destroy_id_node(id_node * idNode)
{
    if(idNode != NULL)
    {
        g_string_free(idNode->id, TRUE);
        free(idNode);
    }
}
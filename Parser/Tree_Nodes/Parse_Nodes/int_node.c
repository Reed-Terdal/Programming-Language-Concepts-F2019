//
// Created by reedt on 10/9/2019.
//

#include <errno.h>
#include <stdio.h>
#include "int_node.h"


int_node * create_int_node(Token * literal, Token * sign)
{
    int_node * retVal = NULL;
    if(literal != NULL && literal->type == t_integer)
    {
        retVal = calloc(1, sizeof(int_node));
        errno = 0;
        retVal->value = g_ascii_strtoll(literal->data->str, NULL, 10);
        if(errno != 0)
        {
            fprintf(stderr, "Syntax Error: Error converting int token to int literal");
            exit(-1);
        }
        if(sign != NULL)
        {
            if(sign->type == t_minus)
            {
                retVal->value *= -1;
            }
            else if(sign->type == t_plus)
            {
                retVal->value *= 1;
            }
            else
            {
                fprintf(stderr, "Syntax Error: Unexpected Token, expected sign");
                exit(-1);
            }
        }
    }
    return retVal;
}

GString * int_node_to_json(int_node * intNode)
{
    GString * retVal = g_string_new(NULL);
    if(intNode != NULL)
    {
        g_string_printf(retVal, "{\"Value\": %ld}", intNode->value);
    }
    else
    {
        g_string_append(retVal, "null");
    }
    return retVal;
}

void destroy_int_node(int_node * intNode)
{
    if(intNode != NULL)
    {
        free(intNode);
    }
}
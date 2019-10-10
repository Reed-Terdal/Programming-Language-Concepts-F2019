//
// Created by reedt on 10/9/2019.
//

#include <errno.h>
#include <stdio.h>
#include "double_node.h"

double_node * create_double_node(Token * literal, Token * sign)
{
    double_node * retVal = NULL;
    if(literal != NULL && literal->type == t_floating)
    {
        retVal = calloc(1, sizeof(double_node));
        errno = 0;
        retVal->value = g_ascii_strtod(literal->data->str, NULL);
        if(errno != 0)
        {
            fprintf(stderr, "Syntax Error: Error converting double token to double literal");
            exit(-1);
        }
        if(sign != NULL)
        {
            if(sign->type == t_minus)
            {
                retVal->value *= -1.0;
            }
            else if(sign->type == t_plus)
            {
                retVal->value *= 1.0;
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

GString * double_node_to_json(double_node * doubleNode)
{
    GString * retVal = g_string_new(NULL);
    if(doubleNode != NULL)
    {
        g_string_printf(retVal, "{\"Value\": %f}", doubleNode->value);
    }
    else
    {
        g_string_append(retVal, "null");
    }
    return retVal;
}

void destroy_double_node(double_node * doubleNode)
{
    if(doubleNode != NULL)
    {
        free(doubleNode);
    }
}
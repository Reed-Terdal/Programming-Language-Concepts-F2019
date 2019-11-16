/**************************************************************************************************
*
* File Name: operator_node.c
*
* Author: Reed Terdal
*
* Created for CS344 on: 10/09/2019
*
* Purpose: Code that provides functions for manipulating operator nodes.
*
**************************************************************************************************/

#include <stdio.h>
#include "operator_node.h"

operator_node * create_operator(Token * opToken)
{
    operator_node * retVal = NULL;

    if(opToken != NULL)
    {
        retVal = calloc(1, sizeof(operator_node));
        switch (opToken->type)
        {
            case t_plus:
                retVal->opType = op_add;
                break;
            case t_minus:
                retVal->opType = op_sub;
                break;
            case t_multiply:
                retVal->opType = op_mult;
                break;
            case t_divide:
                retVal->opType = op_div;
                break;
            case t_power:
                retVal->opType = op_pow;
                break;
            case t_comp_eq:
                retVal->opType = op_comp_eq;
                break;
            case t_comp_neq:
                retVal->opType = op_comp_neq;
                break;
            case t_comp_greater:
                retVal->opType = op_comp_greater;
                break;
            case t_comp_goe:
                retVal->opType = op_comp_goe;
                break;
            case t_comp_less:
                retVal->opType = op_comp_less;
                break;
            case t_comp_loe:
                retVal->opType = op_comp_loe;
                break;
            default:
                fprintf(stderr, "Syntax Error: Unexpected Token when creating operator");
                exit(-1);
        }
    }

    return retVal;
}

GString * operator_to_json(operator_node * anOperator)
{
    GString * retval = g_string_new(NULL);
    if(anOperator != NULL)
    {
        switch (anOperator->opType)
        {
            case op_add:
                g_string_append(retval, "{\"Type\": \"+\"}");
                break;
            case op_sub:
                g_string_append(retval, "{\"Type\": \"-\"}");
                break;
            case op_mult:
                g_string_append(retval, "{\"Type\": \"*\"}");
                break;
            case op_div:
                g_string_append(retval, "{\"Type\": \"/\"}");
                break;
            case op_pow:
                g_string_append(retval, "{\"Type\": \"^\"}");
                break;
            case op_comp_eq:
                g_string_append(retval, "{\"Type\": \"==\"}");
                break;
            case op_comp_neq:
                g_string_append(retval, "{\"Type\": \"!=\"}");
                break;
            case op_comp_goe:
                g_string_append(retval, "{\"Type\": \">=\"}");
                break;
            case op_comp_greater:
                g_string_append(retval, "{\"Type\": \">\"}");
                break;
            case op_comp_loe:
                g_string_append(retval, "{\"Type\": \"<=\"}");
                break;
            case op_comp_less:
                g_string_append(retval, "{\"Type\": \"<\"}");
                break;
            default:
                g_string_append(retval, "{\"Type\": null}");
        }
    }
    else
    {
        g_string_append(retval, "null");
    }
    return retval;
}

void destroy_operator(operator_node * operatorNode)
{
    if(operatorNode != NULL)
    {
        free(operatorNode);
    }
}
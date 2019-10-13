/**************************************************************************************************
*
* File Name: asmt.c
*
* Author: Reed Terdal
*
* Created for CS344 on: 09/25/2019
*
* Purpose: code that provides functions for manipulating asmt nodes.
*
**************************************************************************************************/


#include <stdio.h>
#include "asmt.h"

asmt * create_asmt(GArray * tokenStream, unsigned long index, unsigned long * next)
{
    /**
     * Token[index] is Type (Integer, Double, String)
     * Token[index+1] is ID
     * Token[index+2] is '='
     * Token[index+3 <--> index+n] is expression
     * Token[index+n+1] is ';'
     */

    Token * curToken = &g_array_index(tokenStream, Token, index); // Token is type

    asmt * new_asmt = calloc(1, sizeof(asmt));


    // Need to try expression before we add the ID, because it cannot be self-referencing
    new_asmt->expression = create_expr(tokenStream, index+3, next);
    Type varType;

    switch (curToken->type)
    {
        case t_type_double:
            varType = jdouble;
            if(new_asmt->expression->double_expression == NULL)
            {
                fprintf(stderr, "Syntax Error: Assignment type does not match expression type");
                exit(-1);
            }
            break;
        case t_type_integer:
            varType = jint;
            if(new_asmt->expression->int_expression == NULL)
            {
                fprintf(stderr, "Syntax Error: Assignment type does not match expression type");
                exit(-1);
            }
            break;
        case t_type_string:
            varType = jstring;
            if(new_asmt->expression->string_expression == NULL)
            {
                fprintf(stderr, "Syntax Error: Assignment type does not match expression type");
                exit(-1);
            }
            break;
        default:
            // Unexpected Token, when expecting a Type
            fprintf(stderr, "Syntax Error: Expected a type token");
            exit(-1);
    }

    new_asmt->type = varType;


    // Add the ID to our type properties table
    curToken = &g_array_index(tokenStream, Token, index+1); // Token is ID
    if(!addIDtoTable(curToken->data, varType))
    {
        // We tried to declare a variable that already exists
        fprintf(stderr, "Syntax Error: Tried to create a variable that already exists");
        exit(-1);
    }
    new_asmt->id = create_id_node(curToken);

    curToken = &g_array_index(tokenStream, Token, index+2);
    if(curToken->type != t_assign)
    {
        fprintf(stderr, "Syntax Error: Unexpected Token in assignment");
        exit(-1);
    }

    curToken = &g_array_index(tokenStream, Token, *next);
    if(curToken->type == t_end_stmt)
    {
        (*next)++;
    }
    else
    {
        fprintf(stderr, "Syntax Error: Unexpected Token at end of assignment expression");
        exit(-1);
    }

    return new_asmt;
}

GString * asmt_to_json(asmt * assignment)
{
    GString * retVal = g_string_new(NULL);
    if(assignment != NULL)
    {
        g_string_append(retVal, "{\"ID\": ");
        if(assignment->id != NULL)
        {
            GString * Child = id_node_to_json(assignment->id);
            g_string_append(retVal, Child->str);
            g_string_free(Child, TRUE);
        }
        else
        {
            g_string_append(retVal, "null");
        }

        g_string_append(retVal, ", \"Expression\": ");
        if(assignment->expression != NULL)
        {
            GString * Child = expr_to_json(assignment->expression);
            g_string_append(retVal, Child->str);
            g_string_free(Child, TRUE);
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

void destroy_asmt(asmt * assignment)
{
    if(assignment != NULL)
    {
        if(assignment->expression != NULL)
        {
            destroy_expr(assignment->expression);
        }
        if(assignment->id != NULL)
        {
            destroy_id_node(assignment->id);
        }
        free(assignment);
    }
}
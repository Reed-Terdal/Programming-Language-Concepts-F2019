/**************************************************************************************************
*
* File Name: for_node.c
*
* Author: Jameson Toper
*
* Created for CS344 on: 11/06/2019
*
* Purpose: code provides functions for manipulating for nodes.
*
**************************************************************************************************/


#include "for_node.h"

for_node * create_for_node(GArray * token_stream, unsigned long index, unsigned long * next)
{
    /**
     * Structure:
     * token_stream[index] -> `for`
     * token_stream[index + 1] -> `(`
     * token_stream[index + 2 ... n] -> assignment
     * token_stream[n + 2 ... m] -> conditional
     * token_stream[m + 1 ... p] -> reassignment
     * token_stream[p + 1] -> ')'
     * token_stream[p + 2 ... q] -> b_statement_list
     */
     for_node * retval = calloc(1, sizeof(for_node));

     Token * curToken = &g_array_index(token_stream, Token, index);
     if(curToken->type != t_for)
     {
         fprintf(stderr, "Expected for statement to begin with `for`\n");
         exit(-1);
     }

     curToken = &g_array_index(token_stream, Token, index +1);
     if(curToken->type != t_start_paren)
     {
         fprintf(stderr, "Expected an open paren after `for` keyword\n");
         exit(-1);
     }

     retval->initializer = create_asmt(token_stream, index + 2, next);
     // The asmt should consume the `;`
     retval->conditional = create_i_expr(token_stream, *next, next);
     // the i_expr DOES NOT consume the ';'
     curToken = &g_array_index(token_stream, Token, *next);
     if(curToken->type != t_end_stmt)
     {
         fprintf(stderr, "Expected `;` following condition in for loop\n");
         exit(-1);
     }
    (*next)++;
    retval->reassign = create_r_asmt(token_stream, *next, next);

    curToken = &g_array_index(token_stream, Token, *next);

    if(curToken->type != t_end_paren)
    {
        fprintf(stderr, "Expected ')' at end of for loop\n");
        exit(-1);
    }
    (*next)++;

    retval->body = create_b_stmt_list(token_stream, *next, next);

    return retval;
}

GString * for_node_to_json(for_node * forNode)
{
    GString * retVal = g_string_new(NULL);

    if(forNode != NULL)
    {
        g_string_append(retVal, "{\"Initializer\": ");
        if(forNode->initializer != NULL)
        {
            GString * child = asmt_to_json(forNode->initializer);
            g_string_append(retVal, child->str);
            g_string_free(child, TRUE);
        }
        else
        {
            g_string_append(retVal, "null");
        }

        g_string_append(retVal, ", \"Conditional\": ");
        if(forNode->conditional != NULL)
        {
            GString * child = i_expr_to_json(forNode->conditional);
            g_string_append(retVal, child->str);
            g_string_free(child, TRUE);
        }
        else
        {
            g_string_append(retVal, "null");
        }

        g_string_append(retVal, ", \"Reassignment\": ");
        if(forNode->reassign != NULL)
        {
            GString * child = r_asmt_to_json(forNode->reassign);
            g_string_append(retVal, child->str);
            g_string_free(child, TRUE);
        }
        else
        {
            g_string_append(retVal, "null");
        }

        g_string_append(retVal, ", \"Body\": ");
        if(forNode->body != NULL)
        {
            GString * child = b_stmt_list_to_json(forNode->body);
            g_string_append(retVal, child->str);
            g_string_free(child, TRUE);
        }
        else
        {
            g_string_append(retVal, "null");
        }
        g_string_append_c(retVal, '}');
    }
    return retVal;
}

void destroy_for_node(for_node * forNode)
{
    if(forNode != NULL)
    {
        if(forNode->initializer != NULL)
        {
            destroy_asmt(forNode->initializer);
        }
        if(forNode->conditional != NULL)
        {
            destroy_i_expr(forNode->conditional);
        }
        if(forNode->reassign != NULL)
        {
            destroy_r_asmt(forNode->reassign);
        }
        if(forNode->body != NULL)
        {
            destroy_b_stmt_list(forNode->body);
        }
        free(forNode);
    }
}

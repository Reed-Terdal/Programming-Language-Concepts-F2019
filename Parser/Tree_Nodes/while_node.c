//
// Created by reedt on 11/15/2019.
//

#include "while_node.h"


while_node * create_while_node(GArray * token_stream, unsigned long index, unsigned long * next)
{
    /**
     * Structure:
     * token_stream[index] -> `while`
     * token_stream[index + 1] -> '('
     * token_stream[index + 2 ... n] -> i_expr
     * token_stream[n + 1] -> ')'
     * token_stream[n + 2 ... m] -> b_statement_list
     */
    while_node * retval = calloc(1, sizeof(while_node));

    Token * curToken = &g_array_index(token_stream, Token, index);

    if(curToken->type != t_while)
    {
        fprintf(stderr, "Expected while statement to begin with `while` keyword\n");
        exit(-1);
    }

    curToken = &g_array_index(token_stream, Token, index + 1);
    if(curToken->type != t_start_paren)
    {
        fprintf(stderr, "Expected a opening paren after `while` keyword\n");
        exit(-1);
    }

    retval->conditional = create_i_expr(token_stream, index + 2, next);

    curToken = &g_array_index(token_stream, Token, *next);
    if(curToken->type != t_end_paren)
    {
        fprintf(stderr, "Expected a closing paren after while conditional\n");
        exit(-1);
    }

    retval->body = create_b_stmt_list(token_stream, *next + 1, next);

    return retval;
}

GString * while_node_to_json(while_node * whileNode)
{
    GString * retval = g_string_new(NULL);
    if(whileNode != NULL)
    {
        g_string_append(retval, "{\"Conditional\": ");
        if(whileNode->conditional != NULL)
        {
            GString * child = i_expr_to_json(whileNode->conditional);
            g_string_append(retval, child->str);
            g_string_free(child, TRUE);
        }
        else
        {
            g_string_append(retval, "null");
        }

        g_string_append(retval, ", \"Body\": ");
        if(whileNode->body != NULL)
        {
            GString * child = b_stmt_list_to_json(whileNode->body);
            g_string_append(retval, child->str);
            g_string_free(child, TRUE);
        }
        else
        {
            g_string_append(retval, "null");
        }
        g_string_append_c(retval, '}');
    }
    return retval;
}


void destroy_while_node(while_node * whileNode)
{
    if(whileNode != NULL)
    {
        if(whileNode->conditional != NULL)
        {
            destroy_i_expr(whileNode->conditional);
        }
        if(whileNode->body != NULL)
        {
            destroy_b_stmt_list(whileNode->body);
        }
        free(whileNode);
    }
}
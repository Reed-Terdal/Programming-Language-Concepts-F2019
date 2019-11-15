//
// Created by reedt on 11/15/2019.
//

#include "if_node.h"


if_node * create_if_node(GArray * token_stream, unsigned long index, unsigned long * next)
{
    /**
     * Format:
     * token_stream[index] -> `if`
     * token_stream[index + 1] -> `(`
     * token_stream[index + 2 ... n] -> i_expr
     * token_stream[n + 1] -> ')'
     * token_stream[n + 2 ... m] -> b_stmt_list
     * OPTIONAL
     * token_stream[m + 1] -> `else`
     * toeken_stream[m + 2] -> b_stmt_list
     */

    if_node * ret_val = calloc(1, sizeof(if_node));

    Token * curToken = &g_array_index(token_stream, Token, index);
    if(curToken->type != t_if)
    {
        fprintf(stderr, "Expected if statement to start with `if` keyword\n");
        exit(-1);
    }
    curToken = &g_array_index(token_stream, Token, index + 1);
    if(curToken->type != t_start_paren)
    {
        fprintf(stderr, "Expected if statement to have start paren\n");
        exit(-1);
    }
    ret_val->conditional = create_i_expr(token_stream, index + 2, next);
    curToken = &g_array_index(token_stream, Token, *next);
    if(curToken->type != t_end_paren)
    {
        fprintf(stderr, "Expected close paren after if conditional\n");
        exit(-1);
    }
    ret_val->true_branch = create_b_stmt_list(token_stream, *next + 1, next);
    curToken = &g_array_index(token_stream, Token, *next);
    if(curToken->type == t_else)
    {
        ret_val->false_branch = create_b_stmt_list(token_stream, *next + 1, next);
    }

    return ret_val;
}

GString * if_node_to_json(if_node * ifNode)
{
    GString * retVal = g_string_new(NULL);
    if(ifNode != NULL)
    {
        g_string_append(retVal, "{\"Conditional\": ");
        if(ifNode->conditional != NULL)
        {
            GString * child = i_expr_to_json(ifNode->conditional);
            g_string_append(retVal, child->str);
            g_string_free(child, TRUE);
        }
        else
        {
            g_string_append(retVal, "null");
        }

        g_string_append(retVal, ", \"True_Branch\": ");
        if(ifNode->true_branch != NULL)
        {
            GString * child = b_stmt_list_to_json(ifNode->true_branch);
            g_string_append(retVal, child->str);
            g_string_free(child, TRUE);
        }
        else
        {
            g_string_append(retVal, "null");
        }

        g_string_append(retVal, ", \"False_Branch\": ");
        if(ifNode->false_branch != NULL)
        {
            GString * child = b_stmt_list_to_json(ifNode->false_branch);
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

void destroy_if_node(if_node * ifNode)
{
    if(ifNode != NULL)
    {
        if(ifNode->conditional != NULL)
        {
            destroy_i_expr(ifNode->conditional);
        }
        if(ifNode->true_branch != NULL)
        {
            destroy_b_stmt_list(ifNode->true_branch);
        }
        if(ifNode->false_branch != NULL)
        {
            destroy_b_stmt_list(ifNode->false_branch);
        }
        free(ifNode);
    }
}
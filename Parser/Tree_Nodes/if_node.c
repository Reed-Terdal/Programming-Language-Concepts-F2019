/**************************************************************************************************
*
* File Name: if_node.c
*
* Author: Jameson Toper
*
* Created for CS344 on: 11/06/2019
*
* Purpose: Code that provides functions for manipulating If statement nodes.
*
**************************************************************************************************/

#include <stdio.h>
#include "if_node.h"
#include "b_stmt_list.h"


if_node *create_if_node(GArray *token_stream, unsigned long index, unsigned long *next) {
    if_node *retVal = NULL;
    retVal = calloc(1, sizeof(if_node));
    (*next)++;
    Token *check = &g_array_index(token_stream, Token, *next);
    if (check->type == t_start_paren) {
        (*next)++;
    } else {
        fprintf(stderr,
                "Syntax Error: missing start parend in if statement");
        exit(-1);
    }
    // Grab expression to evaluate
    retVal->expression = create_i_expr(token_stream, *next, next);

    check = &g_array_index(token_stream, Token, *next);
    // Is there an end parend and starting bracket for b_stmt?
    if (check->type == t_end_paren) {
        (*next)++;
        check = &g_array_index(token_stream, Token, *next);
    } else {
        fprintf(stderr,
                "Syntax Error: missing end parend in if statement");
        exit(-1);
    }
    if (check->type == t_start_bracket) {
        (*next)++;
    } else {
        fprintf(stderr,
                "Syntax Error: missing start bracket in if statement");
        exit(-1);
    }

    // Make B_stmt_list for true branch
    retVal->b_true = create_b_stmt_list(token_stream, *next, next);
    check = &g_array_index(token_stream, Token, *next);
    if (check->type == t_end_bracket) {
        (*next)++;
        check = &g_array_index(token_stream, Token, *next);
        if (check->type == t_else) {
            (*next)++;
            check = &g_array_index(token_stream, Token, *next);
            if (check->type != t_start_bracket) {
                fprintf(stderr,
                        "Syntax Error: missing start bracket in else statement");
                exit(-1);
            }
            (*next)++;
            retVal->b_false = create_b_stmt_list(token_stream, *next, next);
            check = &g_array_index(token_stream, Token, *next);
            if (check->type == t_end_bracket) {
                (*next)++;
                return retVal;
            } else {
                fprintf(stderr, "Syntax Error: Missing } at end of else statement");
                exit(-1);

            }
        }
        return retVal;
    } else {
        fprintf(stderr, "Syntax Error: Missing } at end of if statement");
        exit(-1);
    }
}

GString *if_node_to_json(if_node *ifNode) {
    GString *retval = g_string_new(NULL);

    if (retval != NULL) {
        g_string_append(retval, "{\"Expression\": ");
        if (ifNode->expression != NULL) {
            GString *child = i_expr_to_json(ifNode->expression);
            g_string_append(retval, child->str);
            g_string_free(child, TRUE);
        } else {
            g_string_append(retval, "null");
        }
        g_string_append(retval, ", \"True Branch\": ");
        if (ifNode->b_true != NULL) {
            GString *child = b_stmt_list_to_json(ifNode->b_true);
            g_string_append(retval, child->str);
            g_string_free(child, TRUE);
        } else {
            g_string_append(retval, "null");
        }
        g_string_append(retval, ", \"False Branch\": ");
        if (ifNode->b_false != NULL) {
            GString *child = b_stmt_list_to_json(ifNode->b_false);
            g_string_append(retval, child->str);
            g_string_free(child, TRUE);
        } else {
            g_string_append(retval, "null");
        }

        g_string_append_c(retval, '}');

    } else {
        g_string_append(retval, "null");
    }

    return retval;
}

void destroy_if_node(if_node *ifNode) {
    if (ifNode != NULL) {
        if (ifNode->expression)
            destroy_i_expr(ifNode->expression);

        if (ifNode->b_true)
            free(ifNode->b_true);

        if (ifNode->b_false)
            free(ifNode->b_false);
        free(ifNode);
    }
}
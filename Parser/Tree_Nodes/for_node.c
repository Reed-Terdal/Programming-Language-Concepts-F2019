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

#include <stdio.h>
#include "for_node.h"
#include "b_stmt_list.h"
#include "asmt.h"


for_node *create_for_node(GArray *token_stream, unsigned long index, unsigned long *next) {
    for_node *retVal = NULL;

    // Make new for node
    retVal = calloc(1, sizeof(for_node));
    (*next)++;
    Token *check = &g_array_index(token_stream, Token, *next);
    if (check->type == t_start_paren) {
        (*next)++;
    } else {
        fprintf(stderr,
                "Syntax Error: missing start parend in for loop");
        exit(-1);
    }
    // Grab first assignment
    retVal->initialize = create_asmt(token_stream, *next, next);
    // Grab the conditional statement next
    retVal->conditional = create_i_expr(token_stream, *next, next);
    //Check next char
    check = &g_array_index(token_stream, Token, *next);
    if (check->type == t_end_stmt) {
        (*next)++;
    } else {
        fprintf(stderr,
                "Syntax Error: missing second ; in for loop");
        exit(-1);
    }

    // Grab the incrementer reassignment function
    retVal->incrementer = create_r_asmt(token_stream, *next, next);

    // Check next parend
    check = &g_array_index(token_stream, Token, *next);
    if (check->type == t_end_paren) {
        (*next)++;
    } else {
        fprintf(stderr,
                "Syntax Error: missing end parend on for loop");
        exit(-1);
    }

    //Check for closing bracket
    check = &g_array_index(token_stream, Token, *next);
    if (check->type == t_start_bracket) {
        (*next)++;
    } else {
        fprintf(stderr,
                "Syntax Error: missing start bracket in for loop");
        exit(-1);
    }
    retVal->body = create_b_stmt_list(token_stream, *next, next);
    check = &g_array_index(token_stream, Token, (*next));
    if (check->type == t_end_bracket) {
        (*next)++;
        return retVal;
    } else {
        fprintf(stderr, "Syntax Error: Missing } at end of for statement");
        exit(-1);
    }
}

GString *for_node_to_json(for_node *forNode) {
    GString *retval = g_string_new(NULL);

    if (forNode != NULL) {
        g_string_append(retval, "{\"Condition\": ");
        if (forNode->conditional != NULL) {
            GString *child = i_expr_to_json(forNode->conditional);
            g_string_append(retval, child->str);
            g_string_free(child, TRUE);
        } else {
            g_string_append(retval, "null");
        }
        g_string_append(retval, ", \"Incrementer\": ");
        if (forNode->body != NULL) {
            GString *child = r_asmt_to_json(forNode->incrementer);
            g_string_append(retval, child->str);
            g_string_free(child, TRUE);
        } else {
            g_string_append(retval, "null");
        }
        g_string_append(retval, ", \"Body\": ");
        if (forNode->body != NULL) {
            GString *child = b_stmt_list_to_json(forNode->body);
            g_string_append(retval, child->str);
            g_string_free(child, TRUE);
        } else {
            g_string_append(retval, "null");
        }
        g_string_append_c(retval, '}');
    }
    return retval;
}


void destroy_for_node(for_node *forNode) {
    if (forNode != NULL) {
        if (forNode->conditional) {
            free(forNode->conditional);
        }
        if (forNode->incrementer) {
            free(forNode->incrementer);
        }
        if (forNode->initialize) {
            free(forNode->initialize);
        }
        if (forNode->body) {
            free(forNode->body);
        }
        free(forNode);
    }
}
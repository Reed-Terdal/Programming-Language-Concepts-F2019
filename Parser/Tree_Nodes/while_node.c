/**************************************************************************************************
*
* File Name: while_node.c
*
* Author: Jameson Toper
*
* Created while CS344 on: 11/06/2019
*
* Purpose: code provides functions for manipulating while nodes.
*
**************************************************************************************************/


#include <errno.h>
#include <stdio.h>
#include "while_node.h"
#include "b_stmt_list.h"

/**
 * Creates a new while node from the given token stream
 * @param token_stream - Stream of tokens
 * @param index - Where are we in the token list
 * @param next - Lookahead pointer
 * @return new While node with a i_expr conditional and b_stmt_list
 */
while_node *create_while_node(GArray *token_stream, unsigned long index, unsigned long *next) {
    while_node *retVal = NULL;
    Token *check = &g_array_index(token_stream, Token, index);
    //Is it a parend
    if (check->type == t_while) {
        (*next)++;
    } else {
        fprintf(stderr,
                "Syntax Error: missing while decleration");
        exit(-1);
    }
    // Make new While node
    retVal = calloc(1, sizeof(while_node));

    // Load next token
    check = &g_array_index(token_stream, Token, index + 1);
    //Is it a parend
    if (check->type == t_start_paren) {
        (*next)++;
    } else {
        fprintf(stderr,
                "Syntax Error: missing start parend in while statement");
        exit(-1);
    }

    // Grab expression to evaluate
    retVal->conditional = create_i_expr(token_stream, index + 2, next);

    check = &g_array_index(token_stream, Token, *next);

    // Is there an end parend and starting bracket for b_stmt?
    if (check->type == t_end_paren) {
        (*next)++;
        check = &g_array_index(token_stream, Token, *next);
    } else {
        fprintf(stderr,
                "Syntax Error: missing end parend in while statement");
        exit(-1);
    }

    // Make B_stmt_list for body
    retVal->body = create_b_stmt_list(token_stream, *next, next);
    return retVal;
}

/**
 * Convert a while node to json data
 * @param whileNode - While node to process
 * @return - GString holding a json representation of the current state of a while node
 */
GString *while_node_to_json(while_node *whileNode) {
    GString *retval = g_string_new(NULL);

    if (retval != NULL) {
        // Print the conditional part
        g_string_append(retval, "{\"Condition\": ");
        if (whileNode->conditional != NULL) {
            GString *child = i_expr_to_json(whileNode->conditional);
            g_string_append(retval, child->str);
            g_string_free(child, TRUE);
        } else {
            g_string_append(retval, "null");
        }

        // Print the body b_stmt
        g_string_append(retval, ", \"Body\": ");
        if (whileNode->body != NULL) {
            GString *child = b_stmt_list_to_json(whileNode->body);
            g_string_append(retval, child->str);
            g_string_free(child, TRUE);
        } else {
            g_string_append(retval, "null");
        }

        // Complete the json
        g_string_append_c(retval, '}');
    }
    return retval;
}


void destroy_while_node(while_node *whileNode) {
    if (whileNode != NULL) {
        if (whileNode->conditional) {
            destroy_i_expr(whileNode->conditional);
        }
        if (whileNode->body) {
            destroy_b_stmt_list(whileNode->body);
        }
        free(whileNode);
    }
}
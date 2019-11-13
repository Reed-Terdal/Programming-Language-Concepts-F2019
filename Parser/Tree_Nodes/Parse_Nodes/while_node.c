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
while_node *create_while_node(i_expr *conditional, b_stmt_list *body) {
    while_node *retVal = NULL;

    // Make new While node
    retVal = calloc(1, sizeof(while_node));
    retVal->body = body;
    retVal->conditional = conditional;
    //Return the value
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
            free(whileNode->conditional);
        }
        if (whileNode->body) {
            free(whileNode->body);
        }
        free(whileNode);
    }
}
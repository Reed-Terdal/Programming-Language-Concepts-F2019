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


#include <errno.h>
#include <stdio.h>
#include "for_node.h"
#include "b_stmt_list.h"

for_node *create_for_node(asmt *initalizer, i_expr *conditional, r_asmt *incrementer, struct b_stmt_list *body) {
    for_node *retVal = NULL;
    retVal = calloc(1, sizeof(for_node));
    retVal->initialize = initalizer;
    retVal->conditional = conditional;
    retVal->incrementer = incrementer;
    retVal->body = body;
    return retVal;
}

GString *for_node_to_json(for_node *forNode) {
    GString *retval = g_string_new(NULL);

    if (retval != NULL) {
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
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


if_node *create_if_node(i_expr *conditional, struct b_stmt_list *true_branch, struct b_stmt_list *false_branch) {
    if_node *retVal = NULL;
    retVal = calloc(1, sizeof(if_node));
    retVal->expression = conditional;
    retVal->b_true = true_branch;
    retVal->b_false = false_branch;
    return retVal;
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
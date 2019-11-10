/**************************************************************************************************
*
* File Name: stmt_list.c
*
* Author: Reed Terdal
*
* Created for CS344 on: 09/25/2019
*
* Purpose: code that provides functions for manipulating statement list nodes.
*
**************************************************************************************************/

#include "b_stmt_list.h"

b_stmt_list *create_b_stmt_list(GArray *token_stream, unsigned long index, unsigned long *next) {
    b_stmt_list *new_statement_list = calloc(1, sizeof(b_stmt_list));
    new_statement_list->b_statement = create_b_stmt(token_stream, index, next);
    if (*next < token_stream->len) {
        new_statement_list->next = create_b_stmt_list(token_stream, *next, next);
    }
    return new_statement_list;
}

GString *b_stmt_list_to_json(b_stmt_list *b_stmtList) {
    GString *retVal = g_string_new(NULL);
    if (b_stmtList != NULL) {
        g_string_append(retVal, "{\"B_Statement\": ");
        if (b_stmtList->b_statement != NULL) {
            GString *child = b_stmt_to_json(b_stmtList->b_statement);
            g_string_append(retVal, child->str);
            g_string_free(child, TRUE);
        } else {
            g_string_append(retVal, "null");
        }

        g_string_append(retVal, ", \"B_Statement_List\": ");
        if (b_stmtList->next != NULL) {
            GString *child = b_stmt_list_to_json(b_stmtList->next);
            g_string_append(retVal, child->str);
            g_string_free(child, TRUE);
        } else {
            g_string_append(retVal, "null");
        }
        g_string_append_c(retVal, '}');
    } else {
        g_string_append(retVal, "null");
    }
    return retVal;
}

void destroy_b_stmt_list(b_stmt_list *b_stmtList) {
    if (b_stmtList != NULL) {
        if (b_stmtList->b_statement != NULL) {
            destroy_b_stmt(b_stmtList->b_statement);
        }
        if (b_stmtList->next != NULL) {
            destroy_b_stmt_list(b_stmtList->next);
        }
        free(b_stmtList);
    }
}

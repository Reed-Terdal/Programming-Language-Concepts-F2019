/**************************************************************************************************
*
* File Name: f_stmt.c
*
* Author: Jameson Toper
*
* Created for CS344 on: 11/25/2019
*
* Purpose: code that provides functions for manipulating function nodes.
*
**************************************************************************************************/

#include <stdio.h>
#include "f_stmt.h"
#include "stmt.h"

f_stmt *create_f_stmt(GArray *token_stream, unsigned long index, unsigned long *next) {
    f_stmt *new_statement = calloc(1, sizeof(f_stmt));

    // We need to determine the statement type
    // 1. Return
    // 2. Assignment
    // 3. Expression

    Token *curToken = &g_array_index(token_stream, Token, *next);
    if (curToken->type == t_return) {
        new_statement->ret_val = create_expr(token_stream, (*next) + 1, next);
    } else {
        new_statement->statement = create_stmt(token_stream, (*next), next);
        if (*next < token_stream->len) {
            new_statement->next = create_f_stmt(token_stream, *next, next);
        }
    }
    return new_statement;
}

GString *f_stmt_to_json(f_stmt *statement) {
    GString *retVal = g_string_new(NULL);
    if (statement != NULL) {
        g_string_append(retVal, "{\"F Stmt\": ");
        if (statement->statement != NULL) {
            GString *child = stmt_to_json(statement->statement);
            g_string_append(retVal, child->str);
            g_string_free(child, TRUE);
        } else {
            g_string_append(retVal, "null");
        }

        g_string_append(retVal, ", \"Return Value\": ");
        if (statement->ret_val != NULL) {
            GString *child = expr_to_json(statement->ret_val);
            g_string_append(retVal, child->str);
            g_string_free(child, TRUE);
        } else {
            g_string_append(retVal, "null");
        }

        g_string_append_c(retVal, '}');

        if (statement->next) {
            f_stmt_to_json(statement->next);
        }
    } else {
        g_string_append(retVal, "null");
    }
    return retVal;
}

void destroy_f_stmt(f_stmt *statement) {
    if (statement != NULL) {
        if (statement->statement) {
            destroy_stmt(statement->statement);
        }
        if (statement->ret_val) {
            destroy_expr(statement->ret_val);
        }
        if (statement->next) {
            destroy_f_stmt(statement->next);
        }
        free(statement);
    }
}

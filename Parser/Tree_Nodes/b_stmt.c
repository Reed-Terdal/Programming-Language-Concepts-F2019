/**************************************************************************************************
*
* File Name: b_stmt.c
*
* Author: Jameson Toper
*
* Created for CS344 on: 11/06/2019
*
* Purpose: code that provides functions for manipulating b_statement nodes.
*
**************************************************************************************************/

#include <stdio.h>
#include "b_stmt.h"

b_stmt *create_b_stmt(GArray *token_stream, unsigned long index, unsigned long *next) {
    b_stmt *new_statement = calloc(1, sizeof(b_stmt));
    printf("%s\n", g_array_index(token_stream, Token, (*next)).data->str);

    // We need to determine the statement type
    // 1. Reassignment
    // 2. Print
    // 3. Expression

    Token *curToken = &g_array_index(token_stream, Token, index);
    switch (curToken->type) {
        case t_plus:
        case t_minus:
        case t_integer:
        case t_floating:
        case t_string:
            // 3. Expression
            new_statement->expression = create_expr(token_stream, index, next);
            (*next)++;
            break;

        case t_id: {
            // Could be function call or expression
            Type id_type;
            if (findIDType(curToken->data, &id_type)) {
                switch (id_type) {
                    case jf_void:
                        // Function call
                        new_statement->functionCall = create_f_call(token_stream, index, next);
                        curToken = &g_array_index(token_stream, Token, *next);
                        if (curToken->type == t_end_stmt) {
                            (*next)++;
                        } else {
                            fprintf(stderr, "Syntax Error: Unexpected token at end of statement");
                            exit(-1);
                        }
                        break;

                    case jint:
                    case jdouble:
                    case jstring:
                        // Expression
                        new_statement->reassign = create_r_asmt(token_stream, index, next);
                        curToken = &g_array_index(token_stream, Token, *next);

                        if (curToken->type == t_end_stmt) {
                            (*next)++;
                        } else {
                            fprintf(stderr, "Syntax Error: Unexpected token at end of statement");
                            exit(-1);
                        }
                        break;
                    default:
                        fprintf(stderr, "Syntax Error: identifier has an unknown type");
                        exit(-1);
                }
            } else {
                fprintf(stderr, "Syntax Error: Use of undefined identifier in statement");
                exit(-1);
            }
        }
            break;
        case t_end_bracket:
            break;
        case t_if:
            new_statement->ifBlock = create_if_node(token_stream, index, next);
            curToken = &g_array_index(token_stream, Token, *next);
            if (curToken->type == t_end_bracket) {
                (*next)++;
                break;
            } else {
                fprintf(stderr, "Syntax Error: Missing } at end of if statement");
                exit(-1);
            }
        default:
            // Unexpected token when creating Statement, not function call, assignment, or expression
            fprintf(stderr, "Syntax Error: Unexpected Token when creating b_statement %s", curToken->data->str);
            exit(-1);
    }

    return new_statement;
}

GString *b_stmt_to_json(b_stmt *statement) {
    GString *retVal = g_string_new(NULL);
    if (statement != NULL) {
        g_string_append(retVal, "{\"Expression\": ");
        if (statement->expression != NULL) {
            GString *child = expr_to_json(statement->expression);
            g_string_append(retVal, child->str);
            g_string_free(child, TRUE);
        } else {
            g_string_append(retVal, "null");
        }

        g_string_append(retVal, ", \"Assignment\": ");
        if (statement->reassign != NULL) {
            GString *child = r_asmt_to_json(statement->reassign);
            g_string_append(retVal, child->str);
            g_string_free(child, TRUE);
        } else {
            g_string_append(retVal, "null");
        }

        g_string_append(retVal, ", \"Function Call\": ");
        if (statement->functionCall != NULL) {
            GString *child = f_call_to_json(statement->functionCall);
            g_string_append(retVal, child->str);
            g_string_free(child, TRUE);
        } else {
            g_string_append(retVal, "null");
        }
        g_string_append(retVal, ", \"If Statement\": ");
        if (statement->ifBlock != NULL) {
            GString *child = if_node_to_json(statement->ifBlock);
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

void destroy_b_stmt(b_stmt *statement) {
    if (statement != NULL) {
        if (statement->expression != NULL) {
            destroy_expr(statement->expression);
        }
        if (statement->functionCall != NULL) {
            destroy_f_call(statement->functionCall);
        }
        if (statement->reassign != NULL) {
            destroy_r_asmt(statement->reassign);
        }
        if (statement->ifBlock != NULL) {
            destroy_if_node(statement->ifBlock);
        }
        free(statement);
    }
}

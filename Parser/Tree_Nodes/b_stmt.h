/**************************************************************************************************
*
* File Name: b_stmt.h
*
* Author: Jameson Toper
*
* Created for CS344 on: 11/07/2019
*
* Purpose: Header that provides access to functions for manipulating b_statement nodes.
*
**************************************************************************************************/

#ifndef JOTT_INTERPRETTER_B_STMT_H
#define JOTT_INTERPRETTER_B_STMT_H

#include "r_asmt.h"
#include "for_node.h"
#include "while_node.h"
#include "if_node.h"

/**
 * Represents a statement, can be in one of 3 configurations:
 * 1. A function call.
 * 2. A variable assignment.
 * 3. An expression.
 */
typedef struct b_stmt {
    r_asmt *reassign;
    f_call *functionCall;
    if_node *ifBlock;
    for_node *forLoop;
    while_node *whileLoop;
    expr *expression;
} b_stmt;


/**
 * @brief Creates an statement from the token_stream starting at the given index and updates next to be the following
 * token index.
 * @param token_stream The stream of tokens for the entire program.
 * @param index The index in the token stream to start building an statement from.
 * @param next[OUT] A pointer to the index of the next token not part of the statement.
 * @return A newly created statement.
 */
b_stmt *create_b_stmt(GArray *, unsigned long, unsigned long *);

/**
 * @brief Creates a JSON formatted string that represents the provided statement.
 * @param The statement node to create a JSON string for.
 * @return The newly created JSON formatted string.
 */

GString *b_stmt_to_json(b_stmt *);

/**
 * @brief A destructor for an statement, frees any associated memory (recursively).
 * @param The statement to destroy
 */
void destroy_b_stmt(b_stmt *);

#endif //JOTT_INTERPRETTER_BSTMT_H

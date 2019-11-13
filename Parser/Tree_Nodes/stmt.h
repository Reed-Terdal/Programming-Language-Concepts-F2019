/**************************************************************************************************
*
* File Name: stmt.h
*
* Author: Reed Terdal
*
* Created for CS344 on: 09/25/2019
*
* Purpose: Header that provides access to functions for manipulating statement nodes.
*
**************************************************************************************************/

#ifndef JOTT_INTERPRETTER_STMT_H
#define JOTT_INTERPRETTER_STMT_H

#include "f_call.h"
#include "asmt.h"
#include <glib.h>
#include "b_stmt_list.h"

/**
 * Represents a statement, can be in one of 3 configurations:
 * 1. A function call.
 * 2. A variable assignment.
 * 3. An expression.
 */
typedef struct stmt
{
    f_call * function_call;
    asmt * assignment;
    r_asmt *re_asmt;
    expr * expression;
    b_stmt_list *bStmtList;
} stmt;


/**
 * @brief Creates an statement from the token_stream starting at the given index and updates next to be the following
 * token index.
 * @param token_stream The stream of tokens for the entire program.
 * @param index The index in the token stream to start building an statement from.
 * @param next[OUT] A pointer to the index of the next token not part of the statement.
 * @return A newly created statement.
 */
stmt * create_stmt(GArray *, unsigned long, unsigned long *);

/**
 * @brief Creates a JSON formatted string that represents the provided statement.
 * @param The statement node to create a JSON string for.
 * @return The newly created JSON formatted string.
 */

GString * stmt_to_json(stmt *);

/**
 * @brief A destructor for an statement, frees any associated memory (recursively).
 * @param The statement to destroy
 */
void destroy_stmt(stmt *);

#endif //JOTT_INTERPRETTER_STMT_H

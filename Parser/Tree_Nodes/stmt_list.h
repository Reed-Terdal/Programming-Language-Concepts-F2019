/**************************************************************************************************
*
* File Name: stmt_list.h
*
* Author: Reed Terdal
*
* Created for CS344 on: 09/25/2019
*
* Purpose: Header that provides access to functions for manipulating statement list nodes.
*
**************************************************************************************************/

#ifndef JOTT_INTERPRETTER_STMT_LIST_H
#define JOTT_INTERPRETTER_STMT_LIST_H

#include <glib.h>
#include "stmt.h"

typedef struct stmt_list
{
    stmt * statement;
    struct stmt_list * statement_list;
}stmt_list;

/**
 * @brief Creates an statement list from the token_stream starting at the given index and updates next to be the following
 * token index.
 * @param token_stream The stream of tokens for the entire program.
 * @param index The index in the token stream to start building an statement list from.
 * @param next[OUT] A pointer to the index of the next token not part of the statement list.
 * @return A newly created statement list.
 */

stmt_list * create_stmt_list(GArray *, unsigned long, unsigned long *);

/**
 * @brief Creates a JSON formatted string that represents the provided statement list.
 * @param The statement list node to create a JSON string for.
 * @return The newly created JSON formatted string.
 */
GString * stmt_list_to_json(stmt_list *);

/**
 * @brief A destructor for an statement list, frees any associated memory (recursively).
 * @param The statement list to destroy
 */
void destroy_stmt_list(stmt_list *);

#endif //JOTT_INTERPRETTER_STMT_LIST_H

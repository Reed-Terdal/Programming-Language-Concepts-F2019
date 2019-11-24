/**************************************************************************************************
*
* File Name: r_asmt.h
*
* Author: Jameson Toper
*
* Created for CS344 on: 11/08/2019
*
* Purpose: Header that provides access to functions for manipulating reassignment nodes.
*
**************************************************************************************************/

#ifndef JOTT_R_ASMT_H
#define JOTT_R_ASMT_H

#include "id_node.h"
#include "expr.h"
#include <glib.h>

typedef struct r_asmt
{
    id_node * id;
    expr * expression;
} r_asmt;

/**
 * @brief Creates a reassignment from the token_stream starting at the given index and updates next to be the following
 * token index.
 * @param token_stream The stream of tokens for the entire program.
 * @param index The index in the token stream to start building an assignment from.
 * @param next[OUT] A pointer to the index of the next token not part of the assignment.
 * @return A re_assignment function node
 */
r_asmt * create_r_asmt(GArray * token_stream, unsigned long index, unsigned long * next);

/**
 * @brief Creates a JSON formatted string that represents the provided reassignment.
 * @param rAsmt The assignment node to create a JSON string for.
 * @return The newly created JSON formatted string.
 */
GString * r_asmt_to_json(r_asmt * rAsmt);

/**
 * @brief A destructor for the reassignment, frees any associated memory (recursively).
 * @param rAsmt The reassignment to destroy
 */
void destroy_r_asmt(r_asmt * rAsmt);

#endif //JOTT_R_ASMT_H

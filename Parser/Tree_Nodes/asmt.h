/**************************************************************************************************
*
* File Name: asmt.h
*
* Author: Reed Terdal
*
* Created for CS344 on: 09/25/2019
*
* Purpose: Header that provides access to functions for manipulating assignment nodes.
*
**************************************************************************************************/

#ifndef JOTT_INTERPRETTER_ASMT_H
#define JOTT_INTERPRETTER_ASMT_H

#include <stdbool.h>
#include "Token.h"
#include "ids.h"
#include "expr.h"

typedef struct asmt{
    Type type; // Double_type Int_type String_Type
    id_node * id;
    expr * expression; // i_expr, d_expr, s_expr
} asmt;

/**
 * @brief Creates an assignment from the token_stream starting at the given index and updates next to be the following
 * token index.
 * @param token_stream The stream of tokens for the entire program.
 * @param index The index in the token stream to start building an assignment from.
 * @param next[OUT] A pointer to the index of the next token not part of the assignment.
 * @return A newly created assignment.
 */
asmt * create_asmt(GArray * token_stream, unsigned long index, unsigned long * next);

/**
 * @brief Creates a JSON formatted string that represents the provided assignment.
 * @param assignment The assignment node to create a JSON string for.
 * @return The newly created JSON formatted string.
 */
GString * asmt_to_json(asmt * assignment);

/**
 * @brief A destructor for an assignment, frees any associated memory (recursively).
 * @param assignment The assigment to destroy
 */
void destroy_asmt(asmt * assignment);

#endif //JOTT_INTERPRETTER_ASMT_H

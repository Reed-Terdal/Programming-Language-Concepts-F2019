/**************************************************************************************************
*
* File Name: s_expr.h
*
* Author: Reed Terdal
*
* Created for CS344 on: 09/25/2019
*
* Purpose: Header that provides access to functions for manipulating string expression nodes.
*
**************************************************************************************************/


#ifndef JOTT_INTERPRETTER_S_EXPR_H
#define JOTT_INTERPRETTER_S_EXPR_H

#include <Token.h>
#include <stdbool.h>
#include "id_node.h"
#include "string_node.h"
#include "f_call.h"

/**
 * represents a string expression, can be in one of 3 configurations:
 * 1. A literal string
 * 2. An ID for a string variable
 * 3. An ID for a function that returns a string
 */
typedef struct s_expr
{
    // Only one can be non-null
    string_node * literal;
    id_node * id;
    f_call * function_call;
}s_expr;

/**
 * @brief Creates an string expression from the token_stream starting at the given index and updates next to be the following
 * token index.
 * @param token_stream The stream of tokens for the entire program.
 * @param index The index in the token stream to start building an string expression from.
 * @param next[OUT] A pointer to the index of the next token not part of the string expression.
 * @return A newly created string expression.
 */
s_expr * create_s_expr(GArray *, unsigned long, unsigned long *);

/**
 * @brief Creates a JSON formatted string that represents the provided string expression.
 * @param The string expression node to create a JSON string for.
 * @return The newly created JSON formatted string.
 */
GString * s_expr_to_json(s_expr *);

/**
 * @brief A destructor for an string expression, frees any associated memory (recursively).
 * @param The string expression to destroy
 */
void destroy_s_expr(s_expr *);

#endif //JOTT_INTERPRETTER_S_EXPR_H

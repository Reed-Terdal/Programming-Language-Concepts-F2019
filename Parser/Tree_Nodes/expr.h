/**************************************************************************************************
*
* File Name: expr.h
*
* Author: Reed Terdal
*
* Created for CS344 on: 09/25/2019
*
* Purpose: Header that provides access to functions for manipulating expression nodes.
*
**************************************************************************************************/

#ifndef JOTT_INTERPRETTER_EXPR_H
#define JOTT_INTERPRETTER_EXPR_H

#include "i_expr.h"
#include "d_expr.h"
#include "s_expr.h"

typedef struct i_expr i_expr;
typedef struct d_expr d_expr;

/**
 * @note only one can be non-NULL
 */
typedef struct expr{
    i_expr * int_expression;
    s_expr * string_expression;
    d_expr * double_expression;
}expr;

/**
 * @brief Creates an expression from the token_stream starting at the given index and updates next to be the following
 * token index.
 * @param token_stream The stream of tokens for the entire program.
 * @param index The index in the token stream to start building an expression from.
 * @param next[OUT] A pointer to the index of the next token not part of the expression.
 * @return A newly created expression.
 */
expr * create_expr(GArray *, unsigned long, unsigned long *);

/**
 * @brief Creates a JSON formatted string that represents the provided expression.
 * @param The expression node to create a JSON string for.
 * @return The newly created JSON formatted string.
 */
GString * expr_to_json(expr *);

/**
 * @brief A destructor for an expression, frees any associated memory (recursively).
 * @param The expression to destroy
 */
void destroy_expr(expr *);

#endif //JOTT_INTERPRETTER_EXPR_H

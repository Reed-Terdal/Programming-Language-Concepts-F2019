/**************************************************************************************************
*
* File Name: i_expr.h
*
* Author: Reed Terdal
*
* Created for CS344 on: 09/25/2019
*
* Purpose: Header that provides access to functions for manipulating i_exr nodes.
*
**************************************************************************************************/

#ifndef JOTT_INTERPRETTER_I_EXPR_H
#define JOTT_INTERPRETTER_I_EXPR_H

#include "Token.h"
#include "f_call.h"
#include <stdbool.h>
#include "int_node.h"
#include "operator_node.h"
#include "id_node.h"

/**
 * Represents a function call, can have one of 4 configurations:
 * 1. ID to an integer variable
 * 2. A literal integer value
 * 3. A RHS expression, operator, and LHS expression
 * 4. A call to a function returning an integer
 */
typedef struct i_expr
{
    id_node * id;
    int_node * literal;
    operator_node * operatorNode;
    struct i_expr * RHS_expr;
    struct i_expr * LHS_expr;
    f_call * function_call;
}i_expr;

/**
 * @brief Creates an integer expression from the token_stream starting at the given index and updates next to be the following
 * token index.
 * @param token_stream The stream of tokens for the entire program.
 * @param index The index in the token stream to start building an integer expression from.
 * @param next[OUT] A pointer to the index of the next token not part of the integer expression.
 * @return A newly created integer expression.
 */
i_expr * create_i_expr(GArray *, unsigned long, unsigned long *);

/**
 * @brief Creates a JSON formatted string that represents the provided integer expression.
 * @param The integer expression node to create a JSON string for.
 * @return The newly created JSON formatted string.
 */
GString * i_expr_to_json(i_expr *);

/**
 * @brief A destructor for an integer expression, frees any associated memory (recursively).
 * @param The integer expression to destroy
 */
void destroy_i_expr(i_expr *);

#endif //JOTT_INTERPRETTER_I_EXPR_H

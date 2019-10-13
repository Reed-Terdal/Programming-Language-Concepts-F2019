/**************************************************************************************************
*
* File Name: d_expr_node.h
*
* Author: Reed Terdal
*
* Created for CS344 on: 10/09/2019
*
* Purpose: Header that provides access to functions for manipulating ID nodes.
*
**************************************************************************************************/

#ifndef JOTT_INTERPRETTER_D_EXPR_H
#define JOTT_INTERPRETTER_D_EXPR_H

#include "operator_node.h"
#include "id_node.h"
#include "double_node.h"
#include "Token.h"
#include "f_call.h"
#include "Errors.h"

/**
 * @brief This struct represents a double expression and can have one of 4 configurations:
 * 1. ID is populated and all else is NULL.
 * 2. Literal is populated and all else is NULL.
 * 3. RHS, LHS, and op are all populated and all else is NULL.
 * 4. f_call is populated and all else is NULL.
 */
typedef struct d_expr
{
    id_node * id;
    double_node * double_literal;
    operator_node * operator;
    struct d_expr * RHS_expr;
    struct d_expr * LHS_expr;
    f_call * function_call;
}d_expr;

/**
 * @brief Creates a new double expression from the given index in the token stream.
 * @param token_stream The token stream for the entire program.
 * @param index The current index in the token stream to create a double expression from.
 * @param next[OUT] A pointer to the index of the next token not in the new expression.
 * @return a new double expression.
 */
d_expr * create_d_expr(GArray * token_stream, unsigned long index, unsigned long * next);

/**
 * @brief Creates a JSON formatted string that represents the given double expression.
 * @param dExpr The double_expression to create a JSON formatted string for.
 * @return The JSON formatted string.
 */
GString * d_expr_to_json(d_expr * dExpr);

/**
 * @brief a destructor for d_expr, frees any allocated memory (recursively).
 * @param the d_expr to destroy.
 */
void destroy_d_expr(d_expr * dExpr);

#endif //JOTT_INTERPRETTER_D_EXPR_H

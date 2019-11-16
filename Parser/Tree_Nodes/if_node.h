/**************************************************************************************************
*
* File Name: if_node.h
*
* Author: Jameson Toper
*
* Created for CS344 on: 11/06/2019
*
* Purpose: Header that provides access to functions for manipulating if statement nodes.
*
**************************************************************************************************/


#ifndef JOTT_INTERPRETTER_IF_NODE_H
#define JOTT_INTERPRETTER_IF_NODE_H

#include <glib.h>
#include "Token.h"
#include "expr.h"

/**
 * This struct represents an if statement in the parse/syntax tree
 */
typedef struct if_node {
    struct b_stmt_list *b_true;
    struct b_stmt_list *b_false;
    i_expr *expression;
} if_node;

/**
 * @brief This function takes in a true and false statement list and an expression to choose which to run
 * @param b_true[IN] The statement list to execute if the statement is true
 * @param b_false[IN] The statement list to execute if the statement is false
 * @param expression[IN] An expression to evaluate and decide the path to take
 * @return The new if_node.
 */
if_node *create_if_node(GArray *token_stream, unsigned long index, unsigned long *next);

/**
 * @brief This function creates a JSON representation of the provided if_node.
 * @param ifNode[IN] The if_node to create a JSON representation for.
 * @return The JSON formatted string.
 */
GString *if_node_to_json(if_node *ifNode);

/**
 * @brief This function is a destructor responsible for freeing any memory for the provided if_node.
 * @param ifNode[IN/OUT] The if_node to free.
 */
void destroy_if_node(if_node *ifNode);

#endif //JOTT_INTERPRETTER_IF_NODE_H

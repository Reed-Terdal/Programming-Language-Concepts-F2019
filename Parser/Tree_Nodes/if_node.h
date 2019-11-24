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


#ifndef JOTT_IF_NODE_H
#define JOTT_IF_NODE_H

#include "i_expr.h"
#include "b_stmt_list.h"

typedef struct b_stmt b_stmt;

typedef struct if_node
{
    i_expr * conditional;
    b_stmt_list * true_branch;
    b_stmt_list * false_branch;
} if_node;

/**
 * @brief Creates an if node in the parse tree, builds an i_expression, true block and optional false block
 * @param token_stream The token stream of the entire program
 * @param next The index of the next token in the stream, after the if node
 * @return The new if_node.
 */
if_node * create_if_node(GArray * token_stream, unsigned long index, unsigned long * next);

/**
 * @brief This function creates a JSON representation of the provided if_node.
 * @param ifNode[IN] The if_node to create a JSON representation for.
 * @return The JSON formatted string.
 */

GString * if_node_to_json(if_node *);

/**
 * @brief This function is a destructor responsible for freeing any memory for the provided if_node.
 * @param ifNode[IN/OUT] The if_node to free.
 */
void destroy_if_node(if_node *);

#endif //JOTT_IF_NODE_H

/**************************************************************************************************
*
* File Name: int_node.h
*
* Author: Reed Terdal
*
* Created for CS344 on: 10/09/2019
*
* Purpose: Header that provides access to functions for manipulating Int nodes.
*
**************************************************************************************************/

#ifndef JOTT_INTERPRETTER_FOR_NODE_H
#define JOTT_INTERPRETTER_FOR_NODE_H

#include "Token.h"
#include "b_stmt_list.h"
#include "asmt.h"

/**
 * This struct represents a literal integer value as a leaf on the parse/syntax tree.
 */
typedef struct for_node {
    asmt *initialize;
    i_expr *conditional;
    b_stmt_list *body;
} for_node;

/**
 * @brief This function takes in a value token and optionally a sign token and converts them to an int_node.
 * @param value[IN] A token of the type t_int that contains a integer literal in string form.
 * @param sign[IN] An optional sign token to be applied to the literal value, must be (NULL, t_plus, or t_minus).
 * @return The new int_node
 */
for_node *create_for_node(GArray *tokenStream, unsigned long index, unsigned long *next);

/**
 * @brief This function creates a JSON formatted string that represents the int_node provided.
 * @param iNode The iNode to create a JSON string for.
 * @return A JSON formatted string.
 */
GString *for_node_to_json(for_node *forNode);

/**
 * @brief Destructor for int nodes, frees any associated memory.
 * @param iNode the int node to free allocated memory for.
 */
void destroy_for_node(for_node *forNode);

#endif //JOTT_INTERPRETTER_FOR_NODE_H

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

#ifndef JOTT_INTERPRETTER_INT_NODE_H
#define JOTT_INTERPRETTER_INT_NODE_H

#include "Token.h"

/**
 * This struct represents a literal integer value as a leaf on the parse/syntax tree.
 */
typedef struct int_node
{
    gint64 value;
}int_node;

/**
 * @brief This function takes in a value token and optionally a sign token and converts them to an int_node.
 * @param value[IN] A token of the type t_int that contains a integer literal in string form.
 * @param sign[IN] An optional sign token to be applied to the literal value, must be (NULL, t_plus, or t_minus).
 * @return The new int_node
 */
int_node * create_int_node(Token * value, Token * sign);

/**
 * @brief This function creates a JSON formatted string that represents the int_node provided.
 * @param iNode The iNode to create a JSON string for.
 * @return A JSON formatted string.
 */
GString * int_node_to_json(int_node * iNode);

/**
 * @brief Destructor for int nodes, frees any associated memory.
 * @param iNode the int node to free allocated memory for.
 */
void destroy_int_node(int_node * iNode);
#endif //JOTT_INTERPRETTER_INT_NODE_H

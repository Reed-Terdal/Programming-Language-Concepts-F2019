/**************************************************************************************************
*
* File Name: while_node.h
*
* Author: Jameson Toper
*
* Created for CS344 on: 11/06/19
*
* Purpose: Header that provides access to functions for running while nodes.
*
**************************************************************************************************/

#ifndef JOTT_INTERPRETTER_WHILE_NODE_H
#define JOTT_INTERPRETTER_WHILE_NODE_H

#include "Token.h"
#include "i_expr.h"

/**
 * This struct represents a while loop as a leaf on the parse/syntax tree.
 */
typedef struct while_node {
    i_expr *conditional;
    struct b_stmt_list *body;
} while_node;

/**
 * @param value[IN] A token of the condition to continue the loop
 * @param sign[IN] A statement list to run while the condition is true
 * @return The new while_node
 */
while_node *create_while_node(GArray *token_stream, unsigned long index, unsigned long *next);
/**
 * @brief This function creates a JSON formatted string that represents the while_node provided.
 * @param whileNode The whileNode to create a JSON string for.
 * @return A JSON formatted string.
 */
GString *while_node_to_json(while_node *whileNode);

/**
 * @brief Destructor for while nodes, frees any associated memory.
 * @param iNode the while node to free allocated memory for.
 */
void destroy_while_node(while_node *whileNode);

#endif //JOTT_INTERPRETTER_WHILE_NODE_H

/**************************************************************************************************
*
* File Name: int_node.h
*
* Author: Jameson Toper
*
* Created for CS344 on: 11/09/2019
*
* Purpose: Header that provides access to functions for manipulating for nodes.
*
**************************************************************************************************/

#ifndef JOTT_INTERPRETTER_FOR_NODE_H
#define JOTT_INTERPRETTER_FOR_NODE_H

#include <glib.h>
#include "Token.h"
#include "expr.h"
#include "r_asmt.h"
#include "asmt.h"

/**
 * This struct represents a for loop in the parse tree
 */
typedef struct for_node {
    asmt *initialize;
    i_expr *conditional;
    r_asmt *incrementer;
    struct b_stmt_list *body;
} for_node;

/**
 * @brief Creates an assignment from the token_stream starting at the given index and updates next to be the following
 * token index.
 * @param token_stream The stream of tokens for the entire program.
 * @param index The index in the token stream to start building an assignment from.
 * @param next[OUT] A pointer to the index of the next token not part of the assignment.
 * @return A for loop held in a node
 */
for_node *create_for_node(GArray *token_stream, unsigned long index, unsigned long *next);
/**
 * @brief This function creates a JSON formatted string that represents the for_node provided.
 * @param for_node The for_node to create a JSON string for.
 * @return A JSON formatted string.
 */
GString *for_node_to_json(for_node *for_node);

/**
 * @brief Destructor for for nodes, frees any associated memory.
 * @param for_node the for node to free allocated memory for.
 */
void destroy_for_node(for_node *forNode);

#endif //JOTT_INTERPRETTER_FOR_NODE_H

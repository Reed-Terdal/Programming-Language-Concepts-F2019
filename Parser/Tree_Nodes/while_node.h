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


#ifndef JOTT_WHILE_NODE_H
#define JOTT_WHILE_NODE_H

#include <glib.h>
#include "i_expr.h"
#include "b_stmt_list.h"


typedef struct b_stmt b_stmt;
typedef struct b_stmt_list b_stmt_list;


typedef struct while_node
{
    i_expr * conditional;
    b_stmt_list * body;
} while_node;

/**
 * @brief Creates a while loop node in the parse tree
 * @param token_stream The token stream of the entire program
 * @param index The index to start building the while loop from
 * @param next A pointer to the next index in token stream
 * @return A newly created while loop node
 */
while_node * create_while_node(GArray * token_stream, unsigned long index, unsigned long * next);

/**
 * @brief This function creates a JSON formatted string that represents the while_node provided.
 * @param whileNode The whileNode to create a JSON string for.
 * @return A JSON formatted string.
 */
GString * while_node_to_json(while_node *);

/**
 * @brief Destructor for while nodes, frees any associated memory.
 * @param iNode the while node to free allocated memory for.
 */
void destroy_while_node(while_node *);

#endif //JOTT_WHILE_NODE_H

/**************************************************************************************************
*
* File Name: string_node.h
*
* Author: Reed Terdal
*
* Created for CS344 on: 10/09/2019
*
* Purpose: Header that is provides access to functions for manipulating string nodes.
*
**************************************************************************************************/

#ifndef JOTT_INTERPRETTER_STRING_NODE_H
#define JOTT_INTERPRETTER_STRING_NODE_H

#include <glib.h>
#include <Token.h>

typedef struct string_node
{
    GString * value;
} string_node;

/**
 * @brief Constructor for string_nodes, creates a new string from the given token.
 * @param strToken The token to create a string_node for. Must be of type t_string.
 * @return The newly created string_node.
 */
string_node * create_string_node(Token * strToken);

/**
 * @brief Creates a JSON formatted for the given string_node.
 * @param strNode The string_node to create a JSON string for.
 * @return The JSON formatted string.
 */
GString * string_node_to_json(string_node * strNode);

/**
 * @brief Destructor for string_nodes, frees any associated memory.
 * @param strNode The string_node to destroy.
 */
void destroy_string_node(string_node * strNode);

#endif //JOTT_INTERPRETTER_STRING_NODE_H

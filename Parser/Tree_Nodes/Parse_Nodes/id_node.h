/**************************************************************************************************
*
* File Name: id_node.h
*
* Author: Reed Terdal
*
* Created for CS344 on: 10/09/2019
*
* Purpose: Header that provides access to functions for manipulating ID nodes.
*
**************************************************************************************************/


#ifndef JOTT_INTERPRETTER_ID_NODE_H
#define JOTT_INTERPRETTER_ID_NODE_H

#include <glib.h>
#include "Token.h"
#include "ids.h"

/**
 * This struct represents an ID in the parse/syntax tree, including the ID name and type.
 */
typedef struct id_node
{
    GString * id;
    Type type;
}id_node;

/**
 * @brief This function takes in an ID token and creates an id_node.
 * @param idToken[IN] The token to create an id_node for, must be type t_id.
 * @return The new id_node.
 */
id_node * create_id_node(Token * idToken);

/**
 * @brief This function creates a JSON representation of the provided id_node.
 * @param idNode[IN] The id_node to create a JSON representation for.
 * @return The JSON formatted string.
 */
GString * id_node_to_json(id_node * idNode);

/**
 * @brief This function is a destructor responsible for freeing any memory for the provided id_node.
 * @param idNode[IN/OUT] The id_node to free.
 */
void destroy_id_node(id_node * idNode);

#endif //JOTT_INTERPRETTER_ID_NODE_H

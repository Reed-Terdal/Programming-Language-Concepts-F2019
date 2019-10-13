/**************************************************************************************************
*
* File Name: double_node.h
*
* Author: Reed Terdal
*
* Created for CS344 on: 10/09/2019
*
* Purpose: Header that provides access to functions for manipulating double literal values.
*
**************************************************************************************************/

#ifndef JOTT_INTERPRETTER_DOUBLE_NODE_H
#define JOTT_INTERPRETTER_DOUBLE_NODE_H


#include <glib.h>
#include <Token.h>

/**
 * This struct represents a literal double value and would be a leaf in the parse/syntax tree.
 */
typedef struct double_node
{
    gdouble value;
} double_node;

/**
 * @brief This is a constructor that will convert a double token and optionally a sign token into a literal double value.
 * @param value[IN] This token has a string representation of the double value, must be of type t_floating.
 * @param sign[IN] This token is an optional sign to be applied to the value, must be either NULL, t_plue, or t_minus.
 * @return A new double_node with the parsed double literal.
 */
double_node * create_double_node(Token * value, Token * sign);

/**
 * @brief This function provides a JSON representation of the double_node.
 * @param dNode[IN] the double_node to convert to JSON.
 * @return A JSON formatted string that represents the provided double_node.
 */
GString * double_node_to_json(double_node * dNode);

/**
 * @brief This function is a destructor that frees memory associated with the provided double_node.
 * @param dNode[IN/OUT] The double_node to deallocate.
 */
void destroy_double_node(double_node * dNode);

#endif //JOTT_INTERPRETTER_DOUBLE_NODE_H

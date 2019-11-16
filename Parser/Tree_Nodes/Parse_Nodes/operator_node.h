/**************************************************************************************************
*
* File Name: operator_node.h
*
* Author: Reed Terdal
*
* Created for CS344 on: 10/09/2019
*
* Purpose: Header that is provides access to functions for manipulating operator nodes.
*
**************************************************************************************************/


#ifndef JOTT_INTERPRETTER_OPERATOR_NODE_H
#define JOTT_INTERPRETTER_OPERATOR_NODE_H

#include "Token.h"

/**
 * @brief This enum represents the different types of operators.
 */
enum op_type
{
    op_add,
    op_sub,
    op_mult,
    op_div,
    op_pow,
    op_comp_less,
    op_comp_loe, // less than or equal to
    op_comp_eq, // equal
    op_comp_neq, // not equal
    op_comp_goe, // greater than or equal to
    op_comp_greater
};

typedef struct operator_node
{
    enum op_type opType;
}operator_node;

/**
 * @brief constructor for operator_nodes, creates a operator node for a given op token.
 * @param opToken The token to create an operator_node for, must be of type (t_plus, t_minus, t_mult, t_div, t_pow).
 * @return The newly created operator_token
 */
operator_node * create_operator(Token * opToken);

/**
 * @brief Creates a JSON formatted string that represents the given operator_node.
 * @param opNode The operator_node to create a JSON string for.
 * @return The JSON formatted string.
 */
GString * operator_to_json(operator_node * opNode);

/**
 * @brief Destructor for operator_nodes, frees any allocated memory
 * @param opNode The operator_node to destroy.
 */
void destroy_operator(operator_node * opNode);

#endif //JOTT_INTERPRETTER_OPERATOR_NODE_H

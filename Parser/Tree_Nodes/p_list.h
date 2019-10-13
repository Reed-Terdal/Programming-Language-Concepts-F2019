/**************************************************************************************************
*
* File Name: p_list.h
*
* Author: Reed Terdal
*
* Created for CS344 on: 09/26/2019
*
* Purpose: Header that provides access to functions for manipulating parameter list nodes.
*
**************************************************************************************************/

#ifndef JOTT_INTERPRETTER_P_LIST_H
#define JOTT_INTERPRETTER_P_LIST_H

#include <Token.h>
#include "stdbool.h"

typedef struct expr expr;

typedef struct p_list{
    expr * expression;
    struct p_list * next;
}p_list;

/**
 * @brief Creates an parameter list from the token_stream starting at the given index and updates next to be the following
 * token index.
 * @param token_stream The stream of tokens for the entire program.
 * @param index The index in the token stream to start building an parameter list from.
 * @param next[OUT] A pointer to the index of the next token not part of the parameter list.
 * @return A newly created parameter list.
 */
p_list * create_p_list(GArray *, unsigned long, unsigned long *);

/**
 * @brief Creates a JSON formatted string that represents the provided parameter list.
 * @param The parameter list node to create a JSON string for.
 * @return The newly created JSON formatted string.
 */
GString * p_list_to_json(p_list *);

/**
 * @brief A destructor for an parameter list, frees any associated memory (recursively).
 * @param The parameter list to destroy
 */
void destroy_p_list(p_list *);

#endif //JOTT_INTERPRETTER_P_LIST_H

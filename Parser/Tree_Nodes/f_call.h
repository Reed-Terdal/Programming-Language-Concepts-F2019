/**************************************************************************************************
*
* File Name: f_call.h
*
* Author: Reed Terdal
*
* Created for CS344 on: 09/26/2019
*
* Purpose: Header that provides access to functions for manipulating function call nodes.
*
**************************************************************************************************/


#ifndef JOTT_INTERPRETTER_F_CALL_H
#define JOTT_INTERPRETTER_F_CALL_H

#include <Token.h>
#include <stdbool.h>
#include "id_node.h"
#include "p_list.h"


typedef struct f_call{
    id_node * id;
    p_list * params; // Nullable
}f_call;


/**
 * @brief Creates an function call from the token_stream starting at the given index and updates next to be the following
 * token index.
 * @param token_stream The stream of tokens for the entire program.
 * @param index The index in the token stream to start building an function call from.
 * @param next[OUT] A pointer to the index of the next token not part of the function call.
 * @return A newly created function call.
 */
f_call * create_f_call(GArray *, unsigned long, unsigned long *);

/**
 * @brief Creates a JSON formatted string that represents the provided function call.
 * @param The function call node to create a JSON string for.
 * @return The newly created JSON formatted string.
 */
GString * f_call_to_json(f_call *);

/**
 * @brief A destructor for an function call, frees any associated memory (recursively).
 * @param The function call to destroy
 */
void destroy_f_call(f_call *);

#endif //JOTT_INTERPRETTER_F_CALL_H

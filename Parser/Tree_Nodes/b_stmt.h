/**************************************************************************************************
*
* File Name: b_stmt.h
*
* Author: Jameson Toper
*
* Created for CS344 on: 11/07/2019
*
* Purpose: Header that provides access to functions for manipulating b_statement nodes.
*
**************************************************************************************************/

#ifndef JOTT_B_STMT_H
#define JOTT_B_STMT_H

#include <glib.h>
#include "r_asmt.h"
#include "expr.h"
#include "for_node.h"
#include "while_node.h"
#include "if_node.h"

typedef struct if_node if_node;
typedef struct for_node for_node;
typedef struct while_node while_node;

typedef struct b_stmt
{
    r_asmt * reassign;
    f_call * functionCall;
    expr * expression;
    if_node * ifBlock;
    for_node * forLoop;
    while_node * whileLoop;
} b_stmt;

/**
 * @brief Creates a block statement from the token_stream starting at the given index and updates next to be the following
 * token index.
 * @param token_stream The stream of tokens for the entire program.
 * @param index The index in the token stream to start building an statement from.
 * @param next[OUT] A pointer to the index of the next token not part of the block statement.
 * @return A newly created block statement.
 */
b_stmt * create_b_stmt(GArray * token_stream, unsigned long index, unsigned long * next);

/**
 * @brief Creates a JSON formatted string that represents the provided block statement.
 * @param The block statement node to create a JSON string for.
 * @return The newly created JSON formatted string.
 */
GString * b_stmt_to_json(b_stmt *);

/**
 * @brief A destructor for a block statement, frees any associated memory (recursively).
 * @param The block statement to destroy
 */
void destroy_b_stmt(b_stmt *);


#endif //JOTT_B_STMT_H

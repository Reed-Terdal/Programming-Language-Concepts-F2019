/**************************************************************************************************
*
* File Name: program.h
*
* Author: Reed Terdal
*
* Created for CS344 on: 09/25/2019
*
* Purpose: Header that provides access to functions for manipulating program nodes.
*
**************************************************************************************************/


#ifndef JOTT_INTERPRETTER_PROGRAM_H
#define JOTT_INTERPRETTER_PROGRAM_H

#include <glib.h>
#include "stmt_list.h"


typedef struct program {
    stmt_list * statement_list;
} program;

/**
 * @brief Creates an program from the token_stream starting at the given index and updates next to be the following
 * token index.
 * @param token_stream The stream of tokens for the entire program.
 * @param index The index in the token stream to start building an program from.
 * @param next[OUT] A pointer to the index of the next token not part of the program.
 * @return A newly created program.
 */
program * create_program(GArray *, unsigned long, unsigned long *);

/**
 * @brief Creates a JSON formatted string that represents the provided program.
 * @param The program node to create a JSON string for.
 * @return The newly created JSON formatted string.
 */
GString * prog_to_json(program *);

/**
 * @brief A destructor for an program, frees any associated memory (recursively).
 * @param The program to destroy
 */
void destroy_program(program *);

#endif //JOTT_INTERPRETTER_PROGRAM_H

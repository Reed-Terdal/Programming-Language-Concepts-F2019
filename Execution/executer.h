/**************************************************************************************************
*
* File Name: executer.h
*
* Author: Reed Terdal
*
* Created for CS344 on: 10/09/2019
*
* Purpose: Header for the execution phase, exposes the execute function that takes the parse tree and evaluates it.
*
**************************************************************************************************/

#ifndef JOTT_INTERPRETTER_EXECUTER_H
#define JOTT_INTERPRETTER_EXECUTER_H

#include "program.h"

/**
 * @name execute
 * @brief Takes in a parse-tree and executes the program that it represents
 * @param parse_tree The program to be executed
 */
void execute(program *);

#endif //JOTT_INTERPRETTER_EXECUTER_H

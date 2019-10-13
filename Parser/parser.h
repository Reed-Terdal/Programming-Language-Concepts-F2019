/**************************************************************************************************
*
* File Name: parser.h
*
* Author: Reed Terdal
*
* Created for CS344 on: 09/17/2019
*
* Purpose: Header that provides access to creation of a parse tree.
*
**************************************************************************************************/

#ifndef JOTT_INTERPRETTER_PARSER_H
#define JOTT_INTERPRETTER_PARSER_H

#endif //JOTT_INTERPRETTER_PARSER_H

#include <gmodule.h>
#include <stdbool.h>
#include "program.h"


/**
 * @brief This creates a parse tree from the given token stream.
 * @param tokenStream The token stream to create a parse tree for.
 * @return The created parse tree.
 */
program * ParseTokenStream(GArray * tokenStream);
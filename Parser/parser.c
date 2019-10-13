/**************************************************************************************************
*
* File Name: parser.c
*
* Author: Reed Terdal
*
* Created for CS344 on: 09/17/2019
*
* Purpose: Code for creation of a parse tree.
*
**************************************************************************************************/

#include <stdio.h>
#include "parser.h"
#include "Token.h"

program * ParseTokenStream(GArray * tokenStream)
{
    unsigned long end_index = 0;
    program * retVal = create_program(tokenStream, 0, &end_index);
    if(end_index < tokenStream->len)
    {
        fprintf(stderr, "Syntax Error, not all Tokens consumed");
        exit(-1);
    }
    return retVal;
}

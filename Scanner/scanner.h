/**************************************************************************************************
*
* File Name: scanner.h
*
* Author: Reed Terdal
*
* Created for CS344 on: 09/17/2019
*
* Purpose: Header that provides access to the scanner, which tokenize the input program.
*
**************************************************************************************************/


#ifndef JOTT_INTERPRETTER_SCANNER_H
#define JOTT_INTERPRETTER_SCANNER_H

#include <gmodule.h>
#include <stdio.h>
#include <stdint.h>

/**
 * This function scans a Jott file and generates a token stream.
 * @param filePath The path to the file to tokenize
 * @return A pointer to a new GArray of Token structs
 */
GArray * ScanFile(char * filePath);

#endif //JOTT_INTERPRETTER_SCANNER_H

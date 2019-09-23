//
// Created by reedt on 9/17/2019.
//

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

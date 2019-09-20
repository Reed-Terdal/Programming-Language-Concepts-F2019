//
// Created by reedt on 9/17/2019.
//

#ifndef JOTT_INTERPRETTER_SCANNER_H
#define JOTT_INTERPRETTER_SCANNER_H

#include <gmodule.h>
#include <stdio.h>
#include <stdint.h>

enum tokenType : uint8_t
{
    plus = 0,
    minus,
    multiply,
    divide,
    power,
    end_paren,
    start_paren,
    end_stmt,
    assign,
    number,
    id_or_keyword,
    string,
};

typedef struct Token{
    enum tokenType type;
    unsigned int line_num;
    unsigned int col_num;
    char * data;
    unsigned int size;
}Token;

GArray * ScanFile(char * filePath);

#endif //JOTT_INTERPRETTER_SCANNER_H

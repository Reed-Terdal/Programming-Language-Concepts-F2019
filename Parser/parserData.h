//
// Created by rterdal on 9/21/19.
//

#ifndef JOTT_INTERPRETTER_PARSERDATA_H
#define JOTT_INTERPRETTER_PARSERDATA_H

#include <stdbool.h>

typedef enum NodeType
{
    INVALID,
    stmt_list,
    start_paren,
    end_paren,
    end_stmt,
    j_char,
    jl_char,
    ju_char,
    digit,
    sign,
    id,
    stmt,
    j_expr,
    print,
    asmt,
    op,
    dbl,
    d_expr,
    j_int,
    i_expr,
    str_literal,
    str,
    s_expr
}NodeType;

typedef struct Node
{
    NodeType type;
    bool terminal;
    void * data;
} Node;



typedef struct ScannerAction
{
    unsigned int Shift;
    unsigned int Reduce;
    NodeType Reduce_to;
    unsigned int NextState;
    bool accepting;
}ScannerAction;

#endif //JOTT_INTERPRETTER_PARSERDATA_H

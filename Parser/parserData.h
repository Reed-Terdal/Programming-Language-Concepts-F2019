//
// Created by rterdal on 9/21/19.
//

#ifndef JOTT_INTERPRETTER_PARSERDATA_H
#define JOTT_INTERPRETTER_PARSERDATA_H

#include <stdbool.h>

typedef enum NodeType
{
    n_INVALID,
    n_stmt_list,
    n_start_paren,
    n_end_paren,
    n_end_stmt,
    n_char,
    n_l_char,
    n_u_char,
    n_digit,
    n_sign,
    n_id,
    n_stmt,
    n_expr,
    n_print,
    n_asmt,
    n_op,
    n_dbl,
    n_d_expr,
    n_int,
    n_i_expr,
    n_str_literal,
    n_str,
    n_s_expr
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

// TODO FILL THIS IN!!!!
static ScannerAction parseLUT[15][15] = {};

static GQueue parseStack = {NULL, NULL, 0};

#endif //JOTT_INTERPRETTER_PARSERDATA_H

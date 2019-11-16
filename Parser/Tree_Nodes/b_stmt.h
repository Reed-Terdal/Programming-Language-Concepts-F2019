//
// Created by reedt on 11/15/2019.
//

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


b_stmt * create_b_stmt(GArray * token_stream, unsigned long index, unsigned long * next);


GString * b_stmt_to_json(b_stmt *);


void destroy_b_stmt(b_stmt *);


#endif //JOTT_B_STMT_H

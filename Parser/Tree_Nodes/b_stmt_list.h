//
// Created by reedt on 11/15/2019.
//

#ifndef JOTT_B_STMT_LIST_H
#define JOTT_B_STMT_LIST_H

#include "b_stmt.h"
#include <glib.h>

typedef struct b_stmt_list
{
    b_stmt * bStmt;
    struct b_stmt_list * next;
} b_stmt_list;


b_stmt_list * create_b_stmt_list(GArray * token_stream, unsigned long index, unsigned long * next);

GString * b_stmt_list_to_json(b_stmt_list *);

void destroy_b_stmt_list(b_stmt_list *);

#endif //JOTT_B_STMT_LIST_H

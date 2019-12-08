//
// Created by rterdal on 11/24/19.
//

#ifndef JOTT_F_STMT_H
#define JOTT_F_STMT_H

#include <glib.h>
#include "expr.h"

typedef struct f_stmt{
    struct stmt *statement;
    expr * ret_val;
    struct f_stmt * next;
}f_stmt;


f_stmt * create_f_stmt(GArray * token_stream, unsigned long index, unsigned long * next);

GString * f_stmt_to_json(f_stmt *);

void destroy_f_stmt(f_stmt *);

#endif //JOTT_F_STMT_H

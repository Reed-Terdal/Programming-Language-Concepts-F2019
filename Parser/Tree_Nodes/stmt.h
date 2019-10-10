//
// Created by reedt on 9/25/2019.
//

#ifndef JOTT_INTERPRETTER_STMT_H
#define JOTT_INTERPRETTER_STMT_H

#include "f_call.h"
#include "asmt.h"
#include "expr.h"
#include <glib.h>

typedef struct stmt
{
    f_call * function_call;
    asmt * assignment;
    expr * expression;
}stmt;

stmt * create_stmt(GArray *, unsigned long, unsigned long *);

GString * stmt_to_json(stmt *);

void destroy_stmt(stmt *);

#endif //JOTT_INTERPRETTER_STMT_H

//
// Created by reedt on 9/25/2019.
//

#ifndef JOTT_INTERPRETTER_EXPR_H
#define JOTT_INTERPRETTER_EXPR_H

#include "i_expr.h"
#include "d_expr.h"
#include "s_expr.h"

typedef struct expr{
    i_expr * int_expression;
    s_expr * string_expression;
    d_expr * double_expression;
}expr;

expr * create_expr(GArray *, unsigned long, unsigned long *);

GString * expr_to_json(expr *);

void destroy_expr(expr *);

#endif //JOTT_INTERPRETTER_EXPR_H

//
// Created by reedt on 9/25/2019.
//

#ifndef JOTT_INTERPRETTER_I_EXPR_H
#define JOTT_INTERPRETTER_I_EXPR_H

#include "Token.h"
#include "f_call.h"
#include <stdbool.h>

typedef struct i_expr
{
    Token * id;
    Token * int_literal;
    Token * operator;
    struct i_expr * RHS_expr;
    struct i_expr * LHS_expr;
    f_call * function_call;
}i_expr;

i_expr * create_i_expr(GArray *, unsigned long, unsigned long *);

GString * i_expr_to_json(i_expr *);

void destroy_i_expr(i_expr *);

#endif //JOTT_INTERPRETTER_I_EXPR_H

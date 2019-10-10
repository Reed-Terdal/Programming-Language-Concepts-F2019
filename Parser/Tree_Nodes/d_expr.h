//
// Created by reedt on 9/25/2019.
//

#ifndef JOTT_INTERPRETTER_D_EXPR_H
#define JOTT_INTERPRETTER_D_EXPR_H

#include "operator_node.h"
#include "id_node.h"
#include "double_node.h"
#include "Token.h"
#include "f_call.h"

typedef struct d_expr
{
    id_node * id;
    double_node * double_literal;
    operator_node * operator;
    struct d_expr * RHS_expr;
    struct d_expr * LHS_expr;
    f_call * function_call;
}d_expr;

d_expr * create_d_expr(GArray *, unsigned long, unsigned long *);

GString * d_expr_to_json(d_expr *);

void destroy_d_expr(d_expr *);

#endif //JOTT_INTERPRETTER_D_EXPR_H

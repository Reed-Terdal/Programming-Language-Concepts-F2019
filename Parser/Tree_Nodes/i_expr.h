//
// Created by reedt on 9/25/2019.
//

#ifndef JOTT_INTERPRETTER_I_EXPR_H
#define JOTT_INTERPRETTER_I_EXPR_H

#include "Token.h"
#include "f_call.h"
#include <stdbool.h>
#include "int_node.h"
#include "operator_node.h"
#include "id_node.h"

typedef struct i_expr
{
    id_node * id;
    int_node * literal;
    operator_node * operatorNode;
    struct i_expr * RHS_expr;
    struct i_expr * LHS_expr;
    f_call * function_call;
}i_expr;

i_expr * create_i_expr(GArray *, unsigned long, unsigned long *);

GString * i_expr_to_json(i_expr *);

void destroy_i_expr(i_expr *);

#endif //JOTT_INTERPRETTER_I_EXPR_H

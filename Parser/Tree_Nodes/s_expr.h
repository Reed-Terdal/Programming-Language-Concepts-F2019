//
// Created by reedt on 9/25/2019.
//

#ifndef JOTT_INTERPRETTER_S_EXPR_H
#define JOTT_INTERPRETTER_S_EXPR_H

#include <Token.h>
#include <stdbool.h>
#include "id_node.h"
#include "string_node.h"
#include "f_call.h"

typedef struct s_expr
{
    // Only one can be non-null
    string_node * literal;
    id_node * id;
    f_call * function_call;
}s_expr;

s_expr * create_s_expr(GArray *, unsigned long, unsigned long *);

GString * s_expr_to_json(s_expr *);

void destroy_s_expr(s_expr *);

#endif //JOTT_INTERPRETTER_S_EXPR_H

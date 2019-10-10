//
// Created by reedt on 10/9/2019.
//

#ifndef JOTT_INTERPRETTER_OPERATOR_NODE_H
#define JOTT_INTERPRETTER_OPERATOR_NODE_H

#include "Token.h"

enum op_type
{
    op_add,
    op_sub,
    op_mult,
    op_div,
    op_pow
};

typedef struct operator_node
{
    enum op_type opType;
}operator_node;

operator_node * create_operator(Token *);

GString * operator_to_json(operator_node *);

void destroy_operator(operator_node *);

#endif //JOTT_INTERPRETTER_OPERATOR_NODE_H

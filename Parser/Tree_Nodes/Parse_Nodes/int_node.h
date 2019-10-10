//
// Created by reedt on 10/9/2019.
//

#ifndef JOTT_INTERPRETTER_INT_NODE_H
#define JOTT_INTERPRETTER_INT_NODE_H

#include "Token.h"

typedef struct int_node
{
    gint64 value;
}int_node;

int_node * create_int_node(Token *, Token *);

GString * int_node_to_json(int_node *);

void destroy_int_node(int_node *);
#endif //JOTT_INTERPRETTER_INT_NODE_H

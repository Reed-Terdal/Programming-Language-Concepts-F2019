//
// Created by reedt on 10/9/2019.
//

#ifndef JOTT_INTERPRETTER_DOUBLE_NODE_H
#define JOTT_INTERPRETTER_DOUBLE_NODE_H


#include <glib.h>
#include <Token.h>

typedef struct double_node
{
    gdouble value;
} double_node;

double_node * create_double_node(Token *, Token *);

GString * double_node_to_json(double_node *);

void destroy_double_node(double_node *);

#endif //JOTT_INTERPRETTER_DOUBLE_NODE_H

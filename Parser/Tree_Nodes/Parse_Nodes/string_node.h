//
// Created by reedt on 10/9/2019.
//

#ifndef JOTT_INTERPRETTER_STRING_NODE_H
#define JOTT_INTERPRETTER_STRING_NODE_H

#include <glib.h>
#include <Token.h>

typedef struct string_node
{
    GString * value;
} string_node;


string_node * create_string_node(Token *);

GString * string_node_to_json(string_node *);

void destroy_string_node(string_node *);

#endif //JOTT_INTERPRETTER_STRING_NODE_H

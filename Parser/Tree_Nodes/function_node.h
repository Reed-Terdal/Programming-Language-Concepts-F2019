//
// Created by rterdal on 11/24/19.
//

#ifndef JOTT_FUNCTION_NODE_H
#define JOTT_FUNCTION_NODE_H

#include <glib.h>
#include "f_stmt.h"

typedef struct function_node {
    id_node * func_id;
    f_stmt * body;
} function_node;

function_node * create_function_node(GArray * token_stream, unsigned long index, unsigned long * next);

GString * function_node_to_json(function_node *);

void destroy_function_node(function_node *);

#endif //JOTT_FUNCTION_NODE_H

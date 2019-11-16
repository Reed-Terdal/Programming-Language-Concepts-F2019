//
// Created by reedt on 11/15/2019.
//

#ifndef JOTT_WHILE_NODE_H
#define JOTT_WHILE_NODE_H

#include <glib.h>
#include "i_expr.h"
#include "b_stmt_list.h"


typedef struct b_stmt b_stmt;
typedef struct b_stmt_list b_stmt_list;


typedef struct while_node
{
    i_expr * conditional;
    b_stmt_list * body;
} while_node;

while_node * create_while_node(GArray * token_stream, unsigned long index, unsigned long * next);

GString * while_node_to_json(while_node *);

void destroy_while_node(while_node *);

#endif //JOTT_WHILE_NODE_H

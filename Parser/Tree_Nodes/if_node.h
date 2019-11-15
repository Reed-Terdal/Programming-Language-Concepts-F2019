//
// Created by reedt on 11/15/2019.
//

#ifndef JOTT_IF_NODE_H
#define JOTT_IF_NODE_H

#include "i_expr.h"
#include "b_stmt_list.h"

typedef struct b_stmt;

typedef struct if_node
{
    i_expr * conditional;
    b_stmt_list * true_branch;
    b_stmt_list * false_branch;
} if_node;

if_node * create_if_node(GArray * token_stream, unsigned long index, unsigned long * next);

GString * if_node_to_json(id_node *);

void destroy_if_node(if_node *);

#endif //JOTT_IF_NODE_H

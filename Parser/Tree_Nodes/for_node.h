//
// Created by reedt on 11/15/2019.
//

#ifndef JOTT_FOR_NODE_H
#define JOTT_FOR_NODE_H

#include <glib.h>
#include "asmt.h"
#include "i_expr.h"
#include "b_stmt_list.h"
#include "r_asmt.h"

typedef struct b_stmt;

typedef struct for_node
{
    asmt * initializer;
    i_expr * conditional;
    r_asmt * reassign;
    b_stmt_list * body;
} for_node;


for_node * create_for_node(GArray * token_stream, unsigned long index, unsigned long * next);

GString * for_node_to_json(for_node *);

void destroy_for_node(for_node *);

#endif //JOTT_FOR_NODE_H

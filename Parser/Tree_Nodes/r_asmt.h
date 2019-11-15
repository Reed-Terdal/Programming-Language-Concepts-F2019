//
// Created by reedt on 11/15/2019.
//

#ifndef JOTT_R_ASMT_H
#define JOTT_R_ASMT_H

#include "id_node.h"
#include "expr.h"
#include <glib.h>

typedef struct r_asmt
{
    id_node * id;
    expr * expression;
} r_asmt;


r_asmt * create_r_asmt(GArray token_stream, unsigned long index, unsigned long * next);


GString * r_asmt_to_json(r_asmt * rAsmt);


void destroy_r_asmt(r_asmt *);

#endif //JOTT_R_ASMT_H

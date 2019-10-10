//
// Created by reedt on 9/25/2019.
//

#ifndef JOTT_INTERPRETTER_ASMT_H
#define JOTT_INTERPRETTER_ASMT_H

#include <stdbool.h>
#include "Token.h"
#include "ids.h"
#include "expr.h"

typedef struct asmt{
    Type type; // Double_type Int_type String_Type
    id_node * id;
    expr * expression; // i_expr, d_expr, s_expr
} asmt;

asmt * create_asmt(GArray *, unsigned long, unsigned long *);

GString * asmt_to_json(asmt *);

void destroy_asmt(asmt *);

#endif //JOTT_INTERPRETTER_ASMT_H

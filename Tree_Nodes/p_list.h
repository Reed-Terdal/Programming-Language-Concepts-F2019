//
// Created by reedt on 9/26/2019.
//

#ifndef JOTT_INTERPRETTER_P_LIST_H
#define JOTT_INTERPRETTER_P_LIST_H

#include <Token.h>
#include "stdbool.h"

typedef struct expr expr;

typedef struct p_list{
    expr * expression;
    Token * comma;       // This
    struct p_list * next;// and this must both be NULL or non-NULL
}p_list;

p_list * create_p_list(GArray *, unsigned long, unsigned long *);

GString * p_list_to_json(p_list *);

void destroy_p_list(p_list *);

#endif //JOTT_INTERPRETTER_P_LIST_H

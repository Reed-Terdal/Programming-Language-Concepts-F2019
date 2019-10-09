//
// Created by reedt on 9/26/2019.
//

#ifndef JOTT_INTERPRETTER_F_CALL_H
#define JOTT_INTERPRETTER_F_CALL_H

#include <Token.h>
#include <stdbool.h>
#include "p_list.h"



typedef struct f_call{
    Token * id;
    Token * s_paren;
    p_list * params; // Nullable
    Token * e_paren;
}f_call;

f_call * create_f_call(GArray *, unsigned long, unsigned long *);

GString * f_call_to_json(f_call *);

void destroy_f_call(f_call *);

#endif //JOTT_INTERPRETTER_F_CALL_H

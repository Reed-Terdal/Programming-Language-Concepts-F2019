//
// Created by reedt on 9/25/2019.
//

#ifndef JOTT_INTERPRETTER_PROGRAM_H
#define JOTT_INTERPRETTER_PROGRAM_H

#include <glib.h>
#include "stmt_list.h"


typedef struct program {
    stmt_list * statement_list;
} program;


program * create_program(GArray *, unsigned long, unsigned long *);

GString * prog_to_json(program *);

void destroy_program(program *);

#endif //JOTT_INTERPRETTER_PROGRAM_H

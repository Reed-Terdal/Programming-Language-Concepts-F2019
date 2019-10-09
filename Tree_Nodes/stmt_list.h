//
// Created by reedt on 9/25/2019.
//

#ifndef JOTT_INTERPRETTER_STMT_LIST_H
#define JOTT_INTERPRETTER_STMT_LIST_H

#include <glib.h>
#include "stmt.h"

typedef struct stmt_list
{
    stmt * statement;
    struct stmt_list * statement_list;
}stmt_list;


stmt_list * create_stmt_list(GArray *, unsigned long, unsigned long *);

GString * stmt_list_to_json(stmt_list *);

void destroy_stmt_list(stmt_list *);

#endif //JOTT_INTERPRETTER_STMT_LIST_H

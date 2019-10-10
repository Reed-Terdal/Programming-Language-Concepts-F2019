//
// Created by reedt on 10/9/2019.
//

#ifndef JOTT_INTERPRETTER_ID_NODE_H
#define JOTT_INTERPRETTER_ID_NODE_H

#include <glib.h>
#include "Token.h"
#include "ids.h"

typedef struct id_node
{
    GString * id;
    Type type;
}id_node;

id_node * create_id_node(Token * );

GString * id_node_to_json(id_node *);

void destroy_id_node(id_node *);

#endif //JOTT_INTERPRETTER_ID_NODE_H

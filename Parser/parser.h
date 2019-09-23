//
// Created by reedt on 9/17/2019.
//

#ifndef JOTT_INTERPRETTER_PARSER_H
#define JOTT_INTERPRETTER_PARSER_H

#endif //JOTT_INTERPRETTER_PARSER_H

#include <gmodule.h>
#include <stdbool.h>



typedef struct Node
{
    NodeType type;
    bool terminal;
    void * data;
} Node;

GNode * ParseTokenStream(GArray * tokenStream);
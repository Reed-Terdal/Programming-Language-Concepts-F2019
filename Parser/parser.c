//
// Created by reedt on 9/17/2019.
//

#include "parser.h"
#include "parserData.h"
#include "Token.h"

GNode * ParseTokenStream(GArray * tokenStream)
{
    Token token;
    Node * tos;
    GNode * root = g_node_new(NULL);

    g_queue_push_head(&parseStack, &g_array_index(tokenStream, Token, 0));

    for(;;)
    {
        token = g_array_index(tokenStream, Token, 0);
        tos = g_queue_pop_head(&parseStack);

        ScannerAction action = parseLUT[token.type][tos->type];

        if(action.accepting)
        {
            break;
        }
    }


    return root;
}
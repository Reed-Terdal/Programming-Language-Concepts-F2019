/**************************************************************************************************
*
* File Name: for_node.c
*
* Author: Reed Terdal
*
* Created for CS344 on: 10/09/2019
*
* Purpose: code provides functions for manipulating int nodes.
*
**************************************************************************************************/


#include <errno.h>
#include <stdio.h>
#include "for_node.h"


for_node *create_for_node(GArray *tokenStream, unsigned long index, unsigned long *next) {
    for_node *new_for_node = calloc(1, sizeof(for_node));

    // tokenStream[index] --> f_id
    // tokenStream[index+1] --> open paren
    // tokenStream[index+2 ... index+n] --> param comma
    // tokenStream[index+n+1] close paren

    Token *curToken = &g_array_index(tokenStream, Token, index);
    fprintf(stdout, "%s", curToken);
    if (curToken->type == t_id) {
        Type id_type;
        if (findIDType(curToken->data, &id_type)) {
            switch (id_type) {
                case jf_str:
                case jf_void:
                case jf_double:
                case jf_int:
                    new_for_node->initialize = create_id_node(curToken);
                    (*next)++;
                    curToken = &g_array_index(tokenStream, Token, index + 1);
                    if (curToken->type == t_start_paren) {
                        (*next)++;
                    } else {
                        fprintf(stderr, "Syntax Error: Unexpected Token at start of function call");
                        exit(-1);
                    }
                    new_f_call->params = create_p_list(tokenStream, index + 2, next);


                    curToken = &g_array_index(tokenStream, Token, *next);
                    if (curToken->type == t_end_paren) {
                        (*next)++;
                    } else {
                        fprintf(stderr, "Syntax Error: Unexpected Token at end of function call");
                        exit(-1);
                    }
                    break;
                default:
                    fprintf(stderr, "Syntax Error: function call to non-function");
            }
        } else {
            fprintf(stderr, "Syntax Error: called function has not been declared");
        }
    }

}

return
retVal;
}

GString *for_node_to_json(for_node *forNode) {
    GString *retVal = g_string_new(NULL);
    if (forNode != NULL) {
        g_string_printf(retVal, "{\"Initialize\": %ld}", forNode->initialize);
        g_string_printf(retVal, "{\"Conditional\": %ld}", forNode->conditional);
        g_string_printf(retVal, "{\"Body\": %ld}", forNode->body);
    } else {
        g_string_append(retVal, "null");
    }
    return retVal;
}

void destroy_for_node(for_node *forNode) {
    if (forNode != NULL) {
        free(forNode);
    }
}
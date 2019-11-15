//
// Created by reedt on 11/15/2019.
//

#include "for_node.h"

for_node * create_for_node(GArray * token_stream, unsigned long index, unsigned long * next)
{
    /**
     * Structure:
     * token_stream[index] -> `for`
     * token_stream[index + 1] -> `(`
     * token_stream[index + 2 ... n] -> assignment
     * token_stream[n + 1] -> `,`
     * token_stream[n + 2 ... m] -> conditional
     * token_stream[m + 1 ... p] -> reassignment
     * token_stream[p + 1] -> ')'
     * token_stream[p + 2 ... q] -> b_statement_list
     */
     for_node * retval = calloc(1, sizeof(for_node));

     Token * curToken = &g_array_index(token_stream, Token, index);
     if(curToken->type != t_for)
     {
         fprintf(stderr, "Expected for statement to begin with `for`\n");
         exit(-1);
     }

     curToken = &g_array_index(token_stream, Token, index +1);
     if(curToken->type != t_start_paren)
     {
         fprintf(stderr, "Expected an open paren after `for` keyword\n");
         exit(-1);
     }


     return retval;
}

GString * for_node_to_json(for_node *);

void destroy_for_node(for_node *);

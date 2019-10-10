//
// Created by reedt on 9/25/2019.
//

#include "program.h"


program * create_program(GArray * token_stream, unsigned long index, unsigned long * next)
{
    program * new_program = calloc(1, sizeof(program));
    new_program->statement_list = create_stmt_list(token_stream, index, next);
    return new_program;
}

GString * prog_to_json(program * prog)
{
    GString * retVal = g_string_new(NULL);
    if(prog != NULL)
    {
        g_string_append(retVal, "{\"Statement_List\": ");
        if(prog->statement_list != NULL)
        {
            GString * child = stmt_list_to_json(prog->statement_list);
            g_string_append(retVal, child->str);
            g_string_free(child, TRUE);
        }
        else
        {
            g_string_append(retVal, "null");
        }
        g_string_append_c(retVal, '}');
    }
    else
    {
        g_string_append(retVal, "null");
    }
    return retVal;
}

void destroy_program(program * prog)
{
    if(prog != NULL)
    {
        if(prog->statement_list != NULL)
        {
            destroy_stmt_list(prog->statement_list);
        }
        free(prog);
    }
}
/**************************************************************************************************
*
* File Name: stmt_list.c
*
* Author: Reed Terdal
*
* Created for CS344 on: 09/25/2019
*
* Purpose: code that provides functions for manipulating statement list nodes.
*
**************************************************************************************************/


#include "stmt_list.h"

stmt_list * create_stmt_list(GArray * token_stream, unsigned long index, unsigned long * next)
{
    stmt_list * new_statement_list = calloc(1, sizeof(stmt_list));
    new_statement_list->statement = create_stmt(token_stream, index, next);
    if(*next < token_stream->len)
    {
        new_statement_list->statement_list = create_stmt_list(token_stream, *next, next);
    }
    return new_statement_list;
}

GString * stmt_list_to_json(stmt_list * stmtList)
{
    GString * retVal = g_string_new(NULL);
    if(stmtList != NULL)
    {
        g_string_append(retVal, "{\"Statement\": ");
        if(stmtList->statement != NULL)
        {
            GString * child = stmt_to_json(stmtList->statement);
            g_string_append(retVal, child->str);
            g_string_free(child, TRUE);
        }
        else
        {
            g_string_append(retVal, "null");
        }

        g_string_append(retVal, ", \"Statement_List\": ");
        if(stmtList->statement_list != NULL)
        {
            GString * child = stmt_list_to_json(stmtList->statement_list);
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

void destroy_stmt_list(stmt_list * stmtList)
{
    if(stmtList != NULL)
    {
        if(stmtList->statement != NULL)
        {
            destroy_stmt(stmtList->statement);
        }
        if(stmtList->statement_list != NULL)
        {
            destroy_stmt_list(stmtList->statement_list);
        }
        free(stmtList);
    }
}

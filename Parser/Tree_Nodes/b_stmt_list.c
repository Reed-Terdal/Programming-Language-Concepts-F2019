//
// Created by reedt on 11/15/2019.
//

#include "b_stmt_list.h"

b_stmt_list * internal_b_stmt_list_helper(GArray * token_stream, unsigned long index, unsigned long * next);


b_stmt_list * create_b_stmt_list(GArray * token_stream, unsigned long index, unsigned long * next)
{
    Token * curToken = &g_array_index(token_stream, Token, index);
    if(curToken->type != t_start_bracket)
    {
        fprintf(stderr, "Expected starting bracket in block statement\n");
        exit(-1);
    }
    b_stmt_list * ret_val = internal_b_stmt_list_helper(token_stream, index + 1, next);

    curToken = &g_array_index(token_stream, Token, *next);
    if(curToken->type != t_end_bracket)
    {
        fprintf(stderr, "Expected ending bracket after block statement\n");
        exit(-1);
    }
    (*next)++; // consume the end bracket
    return ret_val;
}

b_stmt_list * internal_b_stmt_list_helper(GArray * token_stream, unsigned long index, unsigned long * next)
{
    b_stmt_list * ret_val = calloc(1, sizeof(b_stmt_list));
    ret_val->bStmt = create_b_stmt(token_stream, index, next);

    if(*next < token_stream->len)
    {
        Token * check = &g_array_index(token_stream, Token, *next);
        if(check->type != t_end_bracket)
        {
            ret_val->next = internal_b_stmt_list_helper(token_stream, *next, next);
        }
    }
    return ret_val;
}

GString * b_stmt_list_to_json(b_stmt_list * bStmtList)
{
    GString * retVal = g_string_new(NULL);
    if(bStmtList != NULL)
    {
        g_string_append(retVal, "{\"B_Statement\": ");
        if(bStmtList->bStmt != NULL)
        {
            GString * child = b_stmt_to_json(bStmtList->bStmt);
            g_string_append(retVal, child->str);
            g_string_free(child, TRUE);
        }
        else
        {
            g_string_append(retVal, "null");
        }

        g_string_append(retVal, ", \"B_Statement_List\": ");
        if(bStmtList->next != NULL)
        {
            GString * child = b_stmt_list_to_json(bStmtList->next);
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

void destroy_b_stmt_list(b_stmt_list * bStmtList)
{
    if(bStmtList != NULL)
    {
        if(bStmtList->bStmt != NULL)
        {
            destroy_b_stmt(bStmtList->bStmt);
        }
        if(bStmtList->next != NULL)
        {
            destroy_b_stmt_list(bStmtList->next);
        }
        free(bStmtList);
    }
}
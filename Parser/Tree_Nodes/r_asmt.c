//
// Created by reedt on 11/15/2019.
//

#include "r_asmt.h"


r_asmt * create_r_asmt(GArray * token_stream, unsigned long index, unsigned long * next)
{
    /**
     * Structure:
     * token_stream[index] -> id
     * token_stream[index + 1] -> `=`
     * token_stream[index + 2 ... n] -> expression
     */

    r_asmt * retVal = calloc(1, sizeof(r_asmt));
    Token * curToken = &g_array_index(token_stream, Token, index);
    if(curToken->type != t_id)
    {
        fprintf(stderr, "Expected first token of reassignment to be an ID\n");
        exit(-1);
    }

    Type idType;
    if(!findIDType(curToken->data, &idType))
    {
        fprintf(stderr, "Attempted to reassign variable that has not been declared yet\n");
        exit(-1);
    }
    retVal->id = create_id_node(curToken);

    curToken = &g_array_index(token_stream, Token, index + 1);
    if(curToken->type != t_assign)
    {
        fprintf(stderr, "Expected second token of reassignment to be an `=`\n");
        exit(-1);
    }

    retVal->expression = create_expr(token_stream, index + 2, next);

    if(idType == jint && retVal->expression->int_expression == NULL)
    {
        fprintf(stderr, "Reassignment expression type does not match variable type\n");
        exit(-1);
    }
    if(idType == jdouble && retVal->expression->double_expression == NULL)
    {
        fprintf(stderr, "Reassignment expression type does not match variable type\n");
        exit(-1);
    }
    if(idType == jstring && retVal->expression->string_expression == NULL)
    {
        fprintf(stderr, "Reassignment expression type does not match variable type\n");
        exit(-1);
    }

    return retVal;
}


GString * r_asmt_to_json(r_asmt * rAsmt)
{
    GString * retval = g_string_new(NULL);

    if(rAsmt != NULL)
    {
        g_string_append(retval, "{\"ID\": ");
        if(rAsmt->id != NULL)
        {
            GString * child = id_node_to_json(rAsmt->id);
            g_string_append(retval, child->str);
            g_string_free(child, TRUE);
        }
        else
        {
            g_string_append(retval, "null");
        }

        g_string_append(retval, ", \"Expression\": ");
        if(rAsmt->expression != NULL)
        {
            GString * child = expr_to_json(rAsmt->expression);
            g_string_append(retval, child->str);
            g_string_free(child, TRUE);
        }
        else
        {
            g_string_append(retval, "null");
        }
        g_string_append_c(retval, '}');
    }
    return retval;
}


void destroy_r_asmt(r_asmt * rAsmt)
{
    if(rAsmt != NULL)
    {
        if(rAsmt->id != NULL)
        {
            destroy_id_node(rAsmt->id);
        }
        if(rAsmt->expression != NULL)
        {
            destroy_expr(rAsmt->expression);
        }
        free(rAsmt);
    }
}
//
// Created by reedt on 9/26/2019.
//

#include <stdio.h>
#include <ids.h>
#include "f_call.h"

f_call * create_f_call(GArray * tokenStream, unsigned long index, unsigned long * next)
{
    f_call * new_f_call = calloc(1, sizeof(f_call));

    // tokenStream[index] --> f_id
    // tokenStream[index+1] --> open paren
    // tokenStream[index+2 ... index+n] --> param comma
    // tokenStream[index+n+1] close paren

    Token * curToken = &g_array_index(tokenStream, Token, index);

    if(curToken->type == t_id)
    {
        Type id_type;
        if(findIDType(curToken->data, &id_type))
        {
            switch (id_type)
            {
                case jf_str:
                case jf_void:
                case jf_double:
                case jf_int:
                    new_f_call->id = curToken;
                    (*next)++;
                    curToken = &g_array_index(tokenStream, Token, index+1);
                    if(curToken->type == t_start_paren)
                    {
                        new_f_call->s_paren = curToken;
                        (*next)++;
                    }
                    else
                    {
                        fprintf(stderr, "Syntax Error: Unexpected Token at start of function call");
                        exit(-1);
                    }
                    new_f_call->params = create_p_list(tokenStream, index+2, next);


                    curToken = &g_array_index(tokenStream, Token, *next);
                    if(curToken->type == t_end_paren)
                    {
                        new_f_call->e_paren = curToken;
                        (*next)++;
                    }
                    else
                    {
                        fprintf(stderr, "Syntax Error: Unexpected Token at end of function call");
                        exit(-1);
                    }
                    break;
                default:
                    fprintf(stderr, "Syntax Error: function call to non-function");
            }
        }
        else
        {
            fprintf(stderr, "Syntax Error: called function has not been declared");
        }
    }
    else
    {
        fprintf(stderr, "Syntax Error: unexpected token in function call");
        exit(-1);
    }


    return new_f_call;
}

GString * f_call_to_json(f_call * fCall)
{
    GString * retVal = g_string_new(NULL);
    if(fCall != NULL)
    {
        g_string_append(retVal, "{\"ID\": ");
        if(fCall->id != NULL)
        {
            GString * child = token_to_json(fCall->id);
            g_string_append(retVal, child->str);
            g_string_free(child, TRUE);
        }
        else
        {
            g_string_append(retVal, "null");
        }

        g_string_append(retVal, ", \"Start_Paren\": ");
        if(fCall->s_paren != NULL)
        {
            GString * child = token_to_json(fCall->s_paren);
            g_string_append(retVal, child->str);
            g_string_free(child, TRUE);
        }
        else
        {
            g_string_append(retVal, "null");
        }

        g_string_append(retVal, ", \"Params\": ");
        if(fCall->params != NULL)
        {
            GString * child = p_list_to_json(fCall->params);
            g_string_append(retVal, child->str);
            g_string_free(child, TRUE);
        }
        else
        {
            g_string_append(retVal, "null");
        }

        g_string_append(retVal, ", \"End_Paren\": ");
        if(fCall->e_paren != NULL)
        {
            GString * child = token_to_json(fCall->e_paren);
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

void destroy_f_call(f_call * fCall)
{
    if(fCall != NULL)
    {
        if(fCall->params != NULL)
        {
            destroy_p_list(fCall->params);
        }
        free(fCall);
    }
}

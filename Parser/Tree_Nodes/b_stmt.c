//
// Created by reedt on 11/15/2019.
//

#include "b_stmt.h"


b_stmt * create_b_stmt(GArray * token_stream, unsigned long index, unsigned long * next)
{
    b_stmt * ret_val = calloc(1, sizeof(b_stmt));

    Token * curToken = &g_array_index(token_stream, Token, index);

    switch (curToken->type)
    {
        case t_plus:
        case t_minus:
        case t_integer:
        case t_floating:
        case t_string:
            // 3. Expression
            ret_val->expression = create_expr(token_stream, index, next);
            curToken = &g_array_index(token_stream, Token, *next);
            if(curToken->type == t_end_stmt)
            {
                (*next)++;
            }
            else
            {
                fprintf(stderr, "Syntax Error: Unexpected token at end of statement");
                exit(-1);
            }
            break;
        case t_id:
        {
            // Could be function call or expression
            Type id_type;
            if(findIDType(curToken->data, &id_type))
            {
                switch (id_type)
                {
                    case jf_void:
                        // Function call
                        ret_val->functionCall = create_f_call(token_stream, index, next);
                        curToken = &g_array_index(token_stream, Token, *next);
                        if(curToken->type == t_end_stmt)
                        {
                            (*next)++;
                        }
                        else
                        {
                            fprintf(stderr, "Syntax Error: Unexpected token at end of statement");
                            exit(-1);
                        }
                        break;
                    case jf_str:
                    case jf_double:
                    case jf_int:
                    case jint:
                    case jdouble:
                    case jstring:
                        // Expression
                        ret_val->expression = create_expr(token_stream, index, next);
                        curToken = &g_array_index(token_stream, Token, *next);
                        if(curToken->type == t_end_stmt)
                        {
                            (*next)++;
                        }
                        else
                        {
                            fprintf(stderr, "Syntax Error: Unexpected token at end of statement");
                            exit(-1);
                        }
                        break;
                    default:
                        fprintf(stderr, "Syntax Error: identifier has an unknown type");
                        exit(-1);
                }
            }
            else
            {
                fprintf(stderr, "Syntax Error: Use of undefined identifier in statement");
                exit(-1);
            }
        }
        case t_if:
            ret_val->ifBlock = create_if_node(token_stream, index, next);
            break;
        case t_for:
            ret_val->forLoop = create_for_node(token_stream, index, next);
            break;
        case t_while:
            ret_val->whileLoop = create_while_node(token_stream, index, next);
            break;
        default:
            // Unexpected token when creating Statement, not function call, assignment, or expression
            fprintf(stderr, "Syntax Error: Unexpected Token when creating statement");
            exit(-1);
    }

    return ret_val;
}


GString * b_stmt_to_json(b_stmt *);


void destroy_b_stmt(b_stmt *);
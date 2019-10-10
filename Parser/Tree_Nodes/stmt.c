//
// Created by reedt on 9/25/2019.
//

#include <stdio.h>
#include "stmt.h"
#include "ids.h"

stmt * create_stmt(GArray * token_stream, unsigned long index, unsigned long * next)
{
    stmt * new_statement = calloc(1, sizeof(stmt));

    // We need to determine the statement type
    // 1. Functional call
    // 2. Assignment
    // 3. Expression

    Token * curToken = &g_array_index(token_stream, Token, index);
    switch (curToken->type)
    {
        case t_plus:
        case t_minus:
        case t_integer:
        case t_floating:
        case t_string:
            // 3. Expression
            new_statement->expression = create_expr(token_stream, index, next);
            (*next)++;
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
                        new_statement->function_call = create_f_call(token_stream, index, next);
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
                        new_statement->expression = create_expr(token_stream, index, next);
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
            break;
        case t_type_double:
        case t_type_integer:
        case t_type_string:
            // 2. Assignment
            new_statement->assignment = create_asmt(token_stream, index, next);
            break;
        default:
            // Unexpected token when creating Statement, not function call, assignment, or expression
            fprintf(stderr, "Syntax Error: Unexpected Token when creating statement");
            exit(-1);
    }


    return new_statement;
}

GString * stmt_to_json(stmt * statement)
{
    GString * retVal = g_string_new(NULL);
    if(statement != NULL)
    {
        g_string_append(retVal, "{\"Expression\": ");
        if(statement->expression != NULL)
        {
            GString * child = expr_to_json(statement->expression);
            g_string_append(retVal, child->str);
            g_string_free(child, TRUE);
        }
        else
        {
            g_string_append(retVal, "null");
        }

        g_string_append(retVal, ", \"Assignment\": ");
        if(statement->assignment != NULL)
        {
            GString * child = asmt_to_json(statement->assignment);
            g_string_append(retVal, child->str);
            g_string_free(child, TRUE);
        }
        else
        {
            g_string_append(retVal, "null");
        }

        g_string_append(retVal, ", \"Function_Call\": ");
        if(statement->function_call != NULL)
        {
            GString * child = f_call_to_json(statement->function_call);
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

void destroy_stmt(stmt * statement)
{
    if(statement != NULL)
    {
        if(statement->expression != NULL)
        {
            destroy_expr(statement->expression);
        }
        if(statement->function_call != NULL)
        {
            destroy_f_call(statement->function_call);
        }
        if(statement->assignment != NULL)
        {
            destroy_asmt(statement->assignment);
        }
        free(statement);
    }
}

//
// Created by Jameson Toper on 11/26/19.
//

#ifndef JOTT_FUNCTION_NODE_C
#define JOTT_FUNCTION_NODE_C

#include "function_node.h"


function_node *create_function_node(GArray *token_stream, unsigned long index, unsigned long *next) {
    function_node *new_statement = calloc(1, sizeof(function_node));
    // We need to determine the statement type
    // 1. Functional call
    // 2. Assignment
    // 3. Expression
    Token *name;
    Token *curToken = &g_array_index(token_stream, Token, index);
    Type return_type = t_INVALID;
    switch (curToken->type) {
        case t_type_integer:
            return_type = jf_int;
            break;
        case t_type_double:
            return_type = jf_double;
            break;
        case t_type_string:
            return_type = jf_str;
            break;
        case t_type_void:
            return_type = jf_void;
            break;
        default:
            //error
            break;
    }

    curToken = &g_array_index(token_stream, Token, index + 1);
    if (curToken->type == t_id) {
        name = curToken;
        (*next)++;
    } else {
        fprintf(stderr, "Syntax Error: Unexpected ID defining a function");
    }

    if (g_array_index(token_stream, Token, index + 3).type != t_start_paren) {
        (*next)++;
    } else {
        fprintf(stderr, "Syntax Error: Missing opening parend in function definition");
    }

    GArray *p_list_proto;
    // We create a new array to store gint values.
    // We don't want it zero-terminated or cleared to 0's.
    p_list_proto = g_array_new(FALSE, FALSE, sizeof(named_parameter));

    if (g_array_index(token_stream, Token, (*next) + 2).type != t_end_paren) {
        (*next) += 1;
        while (g_array_index(token_stream, Token, *next).type != t_end_paren) {
            named_parameter *param = calloc(1, sizeof(named_parameter));
            Token *type = &g_array_index(token_stream, Token, (*next));
            Token *var_name = &g_array_index(token_stream, Token, (*next) + 1);

            param->type = (Type) type->type;
            param->name = var_name->data;

            switch (type->type) {
                case t_type_integer:
                    param->type = jf_int;
                case t_type_string:
                    param->type = jf_str;
                case t_type_double:
                    param->type = jf_double;
                default:
                    g_array_append_val (p_list_proto, param);
                    if (g_array_index(token_stream, Token, (*next) + 2).type == t_comma) {
                        (*next) += 3;
                    } else {
                        (*next) += 2;
                    }
                    break;
            }
        }
    }
    curToken = &g_array_index(token_stream, Token, *next);
    if (curToken->type == t_end_paren) {
        (*next)++;
    } else {
        fprintf(stderr, "Syntax Error: Missing end parend in function definition");
        exit(-1);
    }

    addFunctionPrototype(name->data, p_list_proto, return_type);

    parsing_enter_function_scope(name->data);

    curToken = &g_array_index(token_stream, Token, *next);
    if (curToken->type == t_start_bracket) {
        (*next)++;
    } else {
        fprintf(stderr, "Syntax Error: Missing opening bracket in function definition");
        exit(-1);
    }

    new_statement->body = create_f_stmt(token_stream, (*next), next);
    curToken = &g_array_index(token_stream, Token, *next);
    if (curToken->type == t_end_stmt) {
        (*next)++;
    } else {
        fprintf(stderr, "Syntax Error: Unexpected token at end of function");
        exit(-1);
    }

    curToken = &g_array_index(token_stream, Token, *next);
    if (curToken->type == t_end_bracket) {
        (*next)++;
    } else {
        fprintf(stderr, "Syntax Error: Missing closing bracket in function definition");
        exit(-1);
    }
    new_statement->func_id = create_id_node(name);
    parsing_exit_function_scope();
    return new_statement;
}

GString *function_node_to_json(function_node *node) {
    GString *retVal = g_string_new(NULL);
    if (node != NULL) {
        g_string_append(retVal, "{\"Function Name\": ");
        if (node->func_id != NULL) {
            GString *child = id_node_to_json(node->func_id);
            g_string_append(retVal, child->str);
            g_string_free(child, TRUE);
        }
        g_string_append_c(retVal, '}');
        g_string_append(retVal, "{\"Function Statements\": ");
        if (node->body != NULL) {
            GString *child = f_stmt_to_json(node->body);
            g_string_append(retVal, child->str);
            g_string_free(child, TRUE);
        }
        g_string_append_c(retVal, '}');
    } else {
        g_string_append(retVal, "null");
    }
    return retVal;
}

void destroy_function_node(function_node *node) {
    if (node) {
        if (node->func_id) {
            destroy_id_node(node->func_id);
        }
        if (node->body) {
            destroy_f_stmt(node->body);
        }
        free(node);
    }
}


#endif //JOTT_FUNCTION_NODE_C

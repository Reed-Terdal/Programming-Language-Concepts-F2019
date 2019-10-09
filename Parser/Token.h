//
// Created by rterdal on 9/21/19.
//

#ifndef JOTT_INTERPRETTER_TOKEN_H
#define JOTT_INTERPRETTER_TOKEN_H

#include <glib.h>

static const char * tokenTypeStrings[] ={"INVALID", "plus", "minus", "multiply", "divide", "power", "end_paren", "start_paren", "end_stmt", "assign", "integer", "float", "id", "string", "comma", "Double Type", "Int Type", "String Type"};


/**
 * Represents the types of tokens that are supported
 */
typedef enum tokenType
{
    t_INVALID = 0,
    t_plus,
    t_minus,
    t_multiply,
    t_divide,
    t_power,
    t_end_paren,
    t_start_paren,
    t_end_stmt,
    t_assign,
    t_integer,
    t_floating,
    t_id,
    t_string,
    t_comma,
    t_type_double,
    t_type_integer,
    t_type_string
}tokenType;

/**
 * Represents an individual token, including the exact data, type, and location (for debugging)
 */
typedef struct Token{
    tokenType type;
    unsigned int line_num;
    unsigned int col_num;
    GString * data;
}Token;

GString * token_to_json(Token * token);

#endif //JOTT_INTERPRETTER_TOKEN_H

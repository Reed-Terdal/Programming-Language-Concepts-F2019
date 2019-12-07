/**************************************************************************************************
*
* File Name: Token.h
*
* Author: Reed Terdal
*
* Created for CS344 on: 09/21/2019
*
* Purpose: Header that defines tokens that are created from the scanner and consumed by the parser.
*
**************************************************************************************************/

#ifndef JOTT_INTERPRETTER_TOKEN_H
#define JOTT_INTERPRETTER_TOKEN_H

#include <glib.h>

static const char * tokenTypeStrings[] ={"INVALID", "plus", "minus", "multiply", "divide",
                                         "power", "end_paren", "start_paren", "end_stmt", "assign", "integer", "float", "id", "string",
                                         "comma", "Double Type", "Int Type", "String Type", "Less", "Less or equal", "equal", "not equal", "greater or equal", "greater", "for", "while", "if", "else", "start_bracket", "end_bracket"};

char * __filePath;

/**
 * Represents the types of tokens that are supported
 */
typedef enum tokenType
{
    // Phase 1
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
    t_type_string,
    // Phase 2
    t_comp_less,
    t_comp_loe, // less than or equal to
    t_comp_eq, // equal
    t_comp_neq, // not equal
    t_comp_goe, // greater than or equal to
    t_comp_greater,
    t_for,
    t_while,
    t_if,
    t_else,
    t_start_bracket,
    t_end_bracket,
    t_type_void,
    t_return
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

/**
 * @brief Creates a JSON formatted string that represents a Token
 * @param token The token to create a JSON string for
 * @return The JSON formatted string.
 */
GString * token_to_json(Token * token);

#endif //JOTT_INTERPRETTER_TOKEN_H

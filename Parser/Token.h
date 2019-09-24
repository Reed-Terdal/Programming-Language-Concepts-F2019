//
// Created by rterdal on 9/21/19.
//

#ifndef JOTT_INTERPRETTER_TOKEN_H
#define JOTT_INTERPRETTER_TOKEN_H

static const char * tokenTypeStrings[] ={"INVALID","plus","minus","multiply","divide","power","end_paren","start_paren","end_stmt","assign","number","id_or_keyword","string","comma"};


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
    t_number,
    t_id_or_keyword,
    t_string,
    t_comma,
}tokenType;

/**
 * Represents an individual token, including the exact data, type, and location (for debugging)
 */
typedef struct Token{
    tokenType type;
    unsigned int line_num;
    unsigned int col_num;
    char * data;
    unsigned int size;
}Token;

#endif //JOTT_INTERPRETTER_TOKEN_H

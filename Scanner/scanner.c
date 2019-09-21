//
// Created by reedt on 9/17/2019.
//

#include "scanner.h"
#include <errno.h>
#include <stdlib.h>

Token * processCharacter(char c);
typedef enum scannerState
{
    qERR = 0, // having this be 0 makes our list initialization much easier
    q0, // Empty
    q1, // Id or keyword
    q2, // period
    q3, // number (floating)
    q4, // number (integral)
    q5, // = (assignment)
    q6, // start paren
    q7, // end paren
    q8, // end_stmt
    q9, // plus
    q10, // minus
    q11, // multiply
    q12, // divide
    q13, // power
    q14, // start quote
    q15, // end quote
} scannerState;

static scannerState current_state;

#define FINAL_ACCEPT_STATE [0 ... 255]=q0
static scannerState state_LUT[17][256] = {
        /* qERR*/ {},//Default initialized, qERR only leads to qERR
        /* q0  */ {['A'...'Z']=q1, ['a'...'z']=q1, ['.']=q2, ['0'...'9']=q3, ['=']=q5, ['(']=q6, [')']=q7, [';']=q8, ['+']=q9, ['-']=q10, ['*']=q11, ['/']=q12, ['^']=q13, ['"']=q14, [',']=q0, ['\r']=q0, ['\n']=q0, [' ']=q0, ['\t']=q0},
        /* q1  */ {['A'...'Z']=q1, ['a'...'z']=q1,['0'...'9']=q1,   [0 ... '/']=q0, [':'...'@']=q0, ['['...'`']=q0, ['{'...255]=q0},
        /* q2  */ {['0'...'9']=q4,                                  [0 ... '/']=q0, [':'... 255]=q0},
        /* q3  */ {['0'...'9']=q3, ['.']=q4,                        [0 ... '-']=q0, ['/']=q0, [':'... 255]=q0},
        /* q4  */ {['0'...'9']=q4,                                  [0 ... '/']=q0, [':'... 255]=q0},
        /* q5  */ {FINAL_ACCEPT_STATE},
        /* q6  */ {FINAL_ACCEPT_STATE},
        /* q7  */ {FINAL_ACCEPT_STATE},
        /* q8  */ {FINAL_ACCEPT_STATE},
        /* q9  */ {FINAL_ACCEPT_STATE},
        /* q10 */ {FINAL_ACCEPT_STATE},
        /* q11 */ {FINAL_ACCEPT_STATE},
        /* q12 */ {FINAL_ACCEPT_STATE},
        /* q13 */ {FINAL_ACCEPT_STATE},
        /* q14 */ {['A'...'Z']=q14, ['a'...'z']=q14, ['0'...'9']=q14, [' ']=q14, ['"']=q15},
        /* q15 */ {FINAL_ACCEPT_STATE}
};

static tokenType tokenType_LUT[17] = {
        /*qERR*/ INVALID,
        /*q0  */ INVALID,
        /*q1  */ id_or_keyword,
        /*q2  */ INVALID,
        /*q3  */ number,
        /*q4  */ number,
        /*q5  */ assign,
        /*q6  */ start_paren,
        /*q7  */ end_paren,
        /*q8  */ end_stmt,
        /*q9  */ plus,
        /*q10 */ minus,
        /*q11 */ multiply,
        /*q12 */ divide,
        /*q13 */ power,
        /*q14 */ INVALID,
        /*q15 */ string
        };

GArray * ScanFile(char * filePath)
{
    FILE * source = fopen(filePath, "r");
    if (source == NULL)
    {
        fprintf(stderr, "Scanner Error:\nCould not open source file located at: %s\nError Number: %d\n%s\n", filePath, errno, strerror(errno));
        exit(-1);
    }
    current_state = q0;
    GArray * token_stream = g_array_new(TRUE, TRUE, sizeof(Token));

    for(int c = fgetc(source); c != EOF; c = fgetc(source))
    {
        Token * next_token = processCharacter((char)c);
        if(next_token != NULL)
        {
            g_array_append_val(token_stream, *next_token);
            free(next_token);
        }
    }
    if(current_state != q0 && current_state != qERR)
    {
        // Feed in a dummy character in case there is a dangling token
        Token * next_token = processCharacter('\n');
        if(next_token != NULL)
        {
            g_array_append_val(token_stream, *next_token);
            free(next_token);
        }
    }
    if(current_state != q0)
    {
        // If we aren't in q0 the scanning screwed up, throw a fit
        fprintf(stderr, "Scanner Error:\nUnexpected EOF");
        exit(-1);
    }


    return token_stream;
}


Token * processCharacter(char c)
{
    static char * token_buff;
    static unsigned int token_length = 0;
    static unsigned int line_number = 1;
    static unsigned int col_number = 1;
    Token * ret_val = NULL;

    if(current_state != q0 && current_state != qERR)
    {
        scannerState next_state = state_LUT[current_state][c];
        if(next_state != q0 && next_state != qERR)
        {
            // Same state, append our data
            // TODO This isn't the best allocation scheme, maybe revisit this
            token_buff = realloc(token_buff, token_length + 1);
            token_buff[token_length] = c;
            token_length++;
        }
        else if(next_state == q0 && tokenType_LUT[current_state] != INVALID)
        {
            // We have accepted a token, announce it and reset
            Token * new_token = malloc(sizeof(Token));
            new_token->data = token_buff;
            new_token->size = token_length;
            new_token->type = tokenType_LUT[current_state];
            new_token->line_num = line_number;
            new_token->col_num = col_number;
            ret_val = new_token;
            // Resetting
            current_state = q0;
            token_buff = NULL;
            token_length = 0;
        }
        else
        {
            // Something screwed up, time to throw a fit
            fprintf(stderr, "Scanner Error:\nOn line:%d, column:%d\nCurrent state:%d, Current character:%c", line_number, col_number, current_state, c);
            exit(-1);
        }
        current_state = next_state;
    }

    if(current_state == q0)
    {
        // Check next state to make sure we have a good character.
        scannerState next_state = state_LUT[current_state][c];
        if(next_state != q0 && next_state != qERR)
        {
            token_buff = malloc(sizeof(char) * 1);
            token_length = 0;
            token_buff[token_length] = c;
            token_length++;
        }
        else if(next_state == qERR)
        {
            // Something screwed up, time to throw a fit
            fprintf(stderr, "Scanner Error:\nOn line:%d, column:%d\nCurrent state:%d, Current character:%c", line_number, col_number, current_state, c);            exit(-1);
        }
        current_state = next_state;
    }

    // Advance our static tracking vars
    if(c == '\n')
    {
        line_number++;
        col_number = 1;
    }
    else
    {
        col_number++;
    }

    return ret_val;
}

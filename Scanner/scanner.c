//
// Created by reedt on 9/17/2019.
//

#include "scanner.h"
#include <errno.h>
#include <stdlib.h>

Token * processCharacter(char c);
typedef enum scannerState : uint8_t
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

static scannerState next_state[16][256] = {
        /*q0*/ {},
        /*q1*/ {},
        /*q2*/ {},
        /*q3*/ {},
        /*q4*/ {},
        /*q5*/ {},
        /*q6*/ {},
        /*q7*/ {},
        /*q8*/ {},
        /*q9*/ {},
        /*q10*/{},
        /*q11*/{},
        /*q12*/{},
        /*q13*/{},
        /*q14*/{},
        /*q15*/{}
};

GArray * ScanFile(char * filePath)
{
    FILE * source = fopen(filePath, "r");
    if (source == NULL)
    {
        fprintf(stderr, "Scanner Error:\nCould not open source file.\nError Number: %d\n%s\n", errno, strerror(errno));
        exit(-1);
    }
    current_state = q0;
    GArray * token_stream = g_array_new(TRUE, TRUE, sizeof(Token));

    for(int c = fgetc(source); c != EOF; c = fgetc(source))
    {
        Token * next_token = processCharacter((char)c);
        if(next_token != NULL)
        {
            g_array_append_val(token_stream, next_token);
        }
    }


    return token_stream;
}


Token * processCharacter(char c)
{
    static char * token_buff;
    static unsigned int token_length;

    switch(current_state)
    {
        case q0:
        {
            token_buff = malloc(sizeof(char) * 1);
            token_length = 1;
            token_buff[0] = c;
        }
            break;
        case q1:
            break;
        case q2:
            break;
        case q3:
            break;
        case q4:
            break;
        case q5:
            break;
        case q6:
            break;
        case q7:
            break;
        case q8:
            break;
        case q9:
            break;
        case q10:
            break;
        case q11:
            break;
        case q12:
            break;
        case q13:
            break;
        case q14:
            break;
        case q15:
            break;
        default:
            exit(-1);
    }
}

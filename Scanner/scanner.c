//
// Created by reedt on 9/17/2019.
//

#include "scanner.h"
#include "scannerData.h"
#include <errno.h>
#include <stdlib.h>


Token * processCharacter(char c);

GArray * ScanFile(char * filePath)
{
    FILE * source = fopen(filePath, "r");
    if (source == NULL)
    {
        fprintf(stderr, "Scanner Error:\nCould not open source file located at: %s\nError Number: %d\n%s\n", filePath, errno, strerror(errno));
        exit(-1);
    }
    // Reset our scanner's state so that it can process our new file
    current_state = q0;
    GArray * token_stream = g_array_new(TRUE, TRUE, sizeof(Token));

    // Loop through the input file one character at a time until the EOF is reached
    for(int c = fgetc(source); c != EOF; c = fgetc(source))
    {
        Token * next_token = processCharacter((char)c);
        // NULL tokens are expected and common, if the processed character does not terminate a token it will return
        // NULL until another token is passed that will terminate the current token
        if(next_token != NULL)
        {
            // GArray copies input, so we need to free to prevent mem leaks
            g_array_append_val(token_stream, *next_token);
            free(next_token);
        }
    }
    if(current_state != q0 && current_state != qERR)
    {
        // If we are in here it means that we are still processing a token, we need to send in a character to flush it
        // out, a newline will work because it is not part of any token
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

/**
 * This is the meat of the scanner, it processes characters one at a time to build tokens for the language as specified
 * in the DFA
 * @param c The character we are currently reading in from the input file.
 * @return A new Token if the current character terminates the PREVIOUS token, ie input = 3.1415-, the `-` character
 * would cause a Number token to be returned with the content of 3.1415. The next character would of course return a
 * token with the '-'. If a token has not been terminated a NULL ptr will be returned.
 * @warning Will exit with a code of -1 if a scanning error occurs.
 */
Token * processCharacter(char c)
{
    // Static buffer that stores our current token as we build it character by character
    static char * token_buff;
    // Static value tracking length of our current token
    static unsigned int token_length = 0;
    // Static value that tracks the current line number of the program, for debugging scanning errors
    static unsigned int line_number = 1;
    // Static value that tracks the current column number of the program, for debugging scanning errors
    static unsigned int col_number = 1;
    Token * ret_val = NULL;

    if(current_state != q0 && current_state != qERR)
    {
        // If we are in here it means that we are actively working on a token

        scannerState next_state = state_LUT[current_state][c];
        if(next_state != q0 && next_state != qERR)
        {
            // If we are in here, the current character does NOT terminate our current token, so append it and move on
            // TODO This isn't the best allocation scheme, maybe revisit this
            token_buff = realloc(token_buff, token_length + 1);
            token_buff[token_length] = c;
            token_length++;
        }
        else if(next_state == q0 && tokenType_LUT[current_state] != t_INVALID)
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
        // If we are here it means we are ready to start a new token, check the next state to make sure it is valid
        scannerState next_state = state_LUT[current_state][c];
        if(next_state != q0 && next_state != qERR)
        {
            // Time to start a new token
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

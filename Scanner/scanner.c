/**************************************************************************************************
*
* File Name: scanner.c
*
* Author: Reed Terdal
*
* Created for CS344 on: 09/17/2019
*
* Purpose: Code needed for the scanner.
*
**************************************************************************************************/


#include "scanner.h"
#include "scannerData.h"
#include <errno.h>
#include <stdlib.h>
#include "keyword_lookup.h"
#include "Errors.h"

/**
 * @brief Function that prints and exists on a scanning error
 * @param The character that violated scanning.
 * @param The current array of tokens
 */
void scanning_error(char, GArray *);

/**
 * @brief Processes a character and attempts to build a token
 * @param c The current character that is being processed
 * @return a new token or NULL if the end of the token has not been reached.
 */
Token * processCharacter(char c);
/**
 * @brief Identifies what kind of has been found based on the string.
 * @return The type of the token.
 */
tokenType findTokenType(GString *);

GArray * ScanFile(char * filePath)
{
    FILE * source = fopen(filePath, "r");
    if (source == NULL)
    {
        fprintf(stderr, "Scanner Error:\nCould not open source file located at: %s\nError Number: %d\n%s\n", filePath, errno, strerror(errno));
        exit(-1);
    }
    __filePath = filePath;
    // Reset our scanner's state so that it can process our new file
    current_state = q0;
    GArray * token_stream = g_array_new(TRUE, TRUE, sizeof(Token));

    // Loop through the input file one character at a time until the EOF is reached
    for(int c = fgetc(source); c != EOF; c = fgetc(source))
    {
        Token * next_token = processCharacter((char)c);
        // NULL tokens are expected and common, if the processed character does not terminate a token it will return
        // NULL until another token is passed that will terminate the current token
        if(current_state == qERR)
        {
            // Scanner blew up, report an error
            scanning_error((char)c, token_stream);
        }
        else if(next_token != NULL)
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
        Token * next_token = processCharacter('\0');
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

    fclose(source);
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
    static GString * token_buff = NULL;
    // Static value tracking length of our current token
    // Static value that tracks the current line number of the program, for debugging scanning errors
    static unsigned int line_number = 1;
    // Static value that tracks the current column number of the program, for debugging scanning errors
    static unsigned int col_number = 1;
    Token * ret_val = NULL;

    if(current_state != q0 && current_state != qERR)
    {
        // If we are in here it means that we are actively working on a token
        scannerState next_state = state_LUT[current_state][c];
        if(current_state == q17 && next_state == q17)
        {
            // We are processing a comment, don't buffer it
            if (token_buff->len > 0)
            {
                // We buffered the first '/', get rid of it
                g_string_set_size(token_buff, 0);
            }
        }
        else if(next_state != q0 && next_state != qERR)
        {
            // If we are in here, the current character does NOT terminate our current token, so append it and move on
            g_string_append_c(token_buff, c);
        }
        else if(next_state == q0 && tokenType_LUT[current_state] != t_INVALID)
        {
            // We have accepted a token, announce it and reset
            Token * new_token = malloc(sizeof(Token));
            new_token->data = g_string_new(token_buff->str);
            new_token->type = findTokenType(new_token->data);
            new_token->line_num = line_number;
            new_token->col_num = col_number;
            ret_val = new_token;
            // Resetting
            current_state = q0;
            g_string_set_size(token_buff, 0);
        }
        else if(current_state != q17)
        {
            // Something screwed up, time to throw a fit
            previous_state = current_state;
            current_state = qERR;
            return NULL;
        }
        current_state = next_state;
    }

    if(current_state == q0)
    {
        if(c == '\0')
        {
            // We are flushing any remaining tokens, free the allocated space.
            g_string_free(token_buff, TRUE);
            token_buff = NULL;
        }
        else
        {
            scannerState next_state = state_LUT[current_state][c];
            // If we are here it means we are ready to start a new token, check the next state to make sure it is valid
            if(next_state != q0 && next_state != qERR)
            {
                if(token_buff == NULL)
                {
                    token_buff = g_string_new(NULL);
                }
                g_string_set_size(token_buff, 0);
                // Time to start a new token
                g_string_append_c(token_buff, c);
            }
            else if(next_state == qERR)
            {
                // Something screwed up, time to throw a fit
                previous_state = current_state;
                current_state = qERR;
                return NULL;
            }
            current_state = next_state;
        }
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


tokenType findTokenType(GString * buff)
{
    tokenType retVal = tokenType_LUT[current_state];
    struct keyword_lookup * lookup;
    if(current_state == q1 && (lookup = in_word_set(buff->str, buff->len)) != NULL)
    {
        retVal = lookup->type;
    }
    return retVal;
}

void scanning_error(char current_char, GArray * token_stream)
{
    fprintf(stderr, "Scanner Error: ");
    switch (previous_state)
    {
        case q0:
            fprintf(stderr, "Unrecognized character: \"%c\"(0x%x), ", current_char, current_char);
            break;
        case q2:
            fprintf(stderr, "Missing numerical digit following decimal, got \"%c\"(0x%x), ", current_char, current_char);
            break;
        case q14:
            fprintf(stderr, "Missing \" following string literal, got \"%c\"(0x%x), ", current_char, current_char);
            break;
        case q22:
            fprintf(stderr, "Missing \"=\" following !, got \"%c\"(0x%x)", current_char, current_char);
            break;
        default:
            fprintf(stderr, "UNKNOWN ERROR" );
    }
    fprintf(stderr, "\"%s\"", getBrokenStatement(token_stream, token_stream->len)->str);
    fprintf(stderr, " (%s:%d:%d)\n", __filePath, g_array_index(token_stream, Token, token_stream->len-1).line_num, g_array_index(token_stream, Token, token_stream->len -1).col_num);
    exit(-1);
}
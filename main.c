#include <stdio.h>
#include <stdlib.h>
#include "scanner.h"
#include <gmodule.h>

#include <time.h>
#include "Token.h"

void printUsage();
void printToken(Token *);
void printTokens(GArray * tokenStream);


int main(int argc, char ** argv)
{
    if (argc < 2)
    {
        fprintf(stderr, "Missing Jott Source file.\n");
        printUsage();
        return -1;
    }
    long double startTime = (long double)clock() / CLOCKS_PER_SEC;
    GArray * tokenStream = ScanFile(argv[1]);
    long double totalTime = ((long double)clock() / CLOCKS_PER_SEC) - startTime;
    printf("Time to scan: %Lfus\n", (totalTime * 1000) * 1000);

//    printTokens(tokenStream);
    for(unsigned int i = 0; i < tokenStream->len; i++)
    {
        printf("==================%d==============\n", i);
        Token * cur_token = &g_array_index(tokenStream, Token, i);
        printToken(cur_token);
        printf("\n==================%d==============\n", i);
        free(cur_token->data);
    }

    g_array_free(tokenStream, TRUE);



    return 0;
}



void printUsage()
{
    printf("Usage: ./jot <path-to-jott-program>");
}

void printToken(Token * token)
{
    if(token != NULL)
    {
        printf("Type: %s\nLocation: Line=%d, Col=%d\nData: \"%.*s\"", tokenTypeStrings[token->type], token->line_num, token->col_num, token->size, token->data);
    } else
    {
        printf("NULL TOKEN, FREAK OUT");
    }
}

void printTokens(GArray * tokenStream)
{
    for(unsigned int i = 0; i < tokenStream->len; i++)
    {
        Token * cur_token = &g_array_index(tokenStream, Token, i);
        printf("%.*s", cur_token->size, cur_token->data);
        free(cur_token->data);
    }
}
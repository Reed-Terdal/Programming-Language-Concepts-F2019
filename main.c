#include <stdio.h>
#include <glib.h>
#include "Token.h"
#include "scanner.h"
#include "parser.h"
#include "semantic_analyzer.h"
#include <errno.h>

void printUsage();
void printTokens(GArray *);
void cleanup(GArray * tokens, program * tree, GTimer * timer, GString *);
void dumpDebug(GArray *, program *);

int main(int argc, char ** argv)
{
    if (argc < 2)
    {
        fprintf(stderr, "Missing Jott Source file.\n");
        printUsage();
        return -1;
    }
    printf("\n\nFile name: %s\n", argv[1]);

    /// Scanning/Tokenizing
    GTimer * benchTimer = g_timer_new();
    GArray * tokenStream = ScanFile(argv[1]);
    g_timer_stop(benchTimer);
    printf("Time to scan: %.2fus\n", g_timer_elapsed(benchTimer, NULL) * 1000 * 1000);


    /// Parsing
    g_timer_start(benchTimer);
    program * parseTree = ParseTokenStream(tokenStream);
    g_timer_stop(benchTimer);
    printf("Time to parse: %.2fus\n", g_timer_elapsed(benchTimer, NULL) * 1000 * 1000);


    /// Execution
    g_timer_start(benchTimer);
    execute(parseTree);
    g_timer_stop(benchTimer);
    printf("Time to execute: %.2fus\n", g_timer_elapsed(benchTimer, NULL) * 1000 * 1000);


    /// Debug Information
    g_timer_start(benchTimer);
    dumpDebug(tokenStream, parseTree);
    g_timer_stop(benchTimer);
    printf("Time to dump: %.2fus\n", g_timer_elapsed(benchTimer, NULL) * 1000 * 1000);


    /// Clean-up
    cleanup(tokenStream, parseTree, benchTimer, NULL);

    return 0;
}

void printUsage()
{
    printf("Usage: ./jot <path-to-jott-program>");
}

void printTokens(GArray * tokens)
{
    for(unsigned int i = 0; i < tokens->len; i++)
    {
        printf("==================%d==============\n", i);
        Token * token = &g_array_index(tokens, Token, i);
        if(token != NULL)
        {
            printf("Type: %s\nLocation: Line=%d, Col=%d\nData: \"%s\"", tokenTypeStrings[token->type], token->line_num, token->col_num, token->data->str);
        }
        else
        {
            printf("NULL TOKEN, FREAK OUT");
        }
        printf("\n==================%d==============\n", i);
    }

}

void cleanup(GArray * tokens, program * tree, GTimer * timer, GString * json)
{
    if(json != NULL)
    {
        g_string_free(json, TRUE);
    }
    if(tree != NULL)
    {
        destroy_program(tree);
    }
    if(timer != NULL)
    {
        g_timer_destroy(timer);
    }
    if(tokens != NULL)
    {
        for(unsigned int i = 0; i < tokens->len; i++)
        {
            g_string_free(g_array_index(tokens, Token, i).data, TRUE);
        }
        g_array_free(tokens, TRUE);
    }
}

void dumpDebug(GArray * tokenStream, program * prog)
{
    FILE * out = fopen("./dump.json", "w");
    if( out == NULL)
    {
        printf("Debug dump failed with Error Num: %d\n", errno);
        return;
    }
    GString * json = g_string_new(NULL);
    g_string_append(json, "{ \"Tokens\": [");
    for(unsigned int i = 0; i < tokenStream->len; i++)
    {
        GString * child = token_to_json(&g_array_index(tokenStream, Token, i));
        g_string_append(json, child->str);
        g_string_free(child, TRUE);
        g_string_append(json, ", ");
    }
    g_string_truncate(json, json->len - 2 ); // removes the last ','
    g_string_append(json, "], \"Parse_Tree\": {\"Program\": ");
    GString * child = prog_to_json(prog);
    g_string_append(json, child->str);
    g_string_free(child, TRUE);
    g_string_append(json, "}}");

    fputs(json->str, out);
    fflush(out);
    fclose(out);
    g_string_free(json, TRUE);
}

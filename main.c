#include <stdio.h>
#include "executer.h"
#include <glib.h>
#include "Token.h"
#include "scanner.h"
#include "parser.h"
#include <errno.h>

/**
 * @brief Prints how to use the program
 */
void printUsage();
/**
 * @brief Frees all memory for the program.
 * @param tokens The token stream of the program
 * @param tree The parse tree of the program
 * @param timer The timer used for benchmarking
 */
void cleanup(GArray * tokens, program * tree, GTimer * timer, GString *);
/**
 * @brief writes a JSON file with debug information about the program
 * @param The token stream of the program
 * @param The parse tree of the program
 * @param A string with metadata about the program
 */
void dumpDebug(GArray *, program *, GString *);

int main(int argc, char ** argv)
{
    if (argc < 2)
    {
        fprintf(stderr, "Missing Jott Source file.\n");
        printUsage();
        return -1;
    }

    GString * metadata = g_string_new(NULL);
    g_string_printf(metadata,"{ \"File_Name\": \"%s\", ", argv[1]);

    /// Scanning/Tokenizing
    GTimer * benchTimer = g_timer_new();
    GArray * tokenStream = ScanFile(argv[1]);
    g_timer_stop(benchTimer);
    g_string_append_printf(metadata, "\"Scan Time(us)\": %.2f, ", g_timer_elapsed(benchTimer, NULL) * 1000 * 1000);


    /// Parsing
    g_timer_start(benchTimer);
    program * parseTree = ParseTokenStream(tokenStream);
    g_timer_stop(benchTimer);
    g_string_append_printf(metadata, "\"Parse Time(us)\": %.2f, ", g_timer_elapsed(benchTimer, NULL) * 1000 * 1000);


    /// Execution
    g_timer_start(benchTimer);
    execute(parseTree->statement_list);
    g_timer_stop(benchTimer);
    g_string_append_printf(metadata, "\"Run Time(us)\": %.2f, ", g_timer_elapsed(benchTimer, NULL) * 1000 * 1000);


    g_string_append_printf(metadata, "\"Token Count\": %u}", tokenStream->len);

    if(argc >= 3 && strcmp(argv[2], "-d") == 0)
    {
        /// Debug Information
        printf("Writing dumping debug info...\n");
        dumpDebug(tokenStream, parseTree, metadata);
    }
    g_string_free(metadata, TRUE);


    /// Clean-up
    cleanup(tokenStream, parseTree, benchTimer, NULL);
    return 0;
}

void printUsage()
{
    printf("Usage: ./jott <path-to-jott-program>");
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

void dumpDebug(GArray * tokenStream, program * prog, GString * metadata)
{
    static int count = 0;

    char buf[20];

    if(count == 0)
    {
        sprintf(buf, "./dump.json");
    }
    else
    {
        sprintf(buf, "./dump(%d).json", count);
    }

    FILE * out = fopen(buf, "w");
    if( out == NULL)
    {
        printf("Debug dump failed with Error Num: %d\n", errno);
        return;
    }

    GString * json = g_string_new("{");

    if(metadata != NULL)
    {
        g_string_append_printf(json, "\"Metadata\": %s,", metadata->str);
    }

    if(tokenStream != NULL)
    {
        g_string_append(json, "\"Tokens\": [");
        for(unsigned int i = 0; i < tokenStream->len; i++)
        {
            GString * child = token_to_json(&g_array_index(tokenStream, Token, i));
            g_string_append(json, child->str);
            g_string_free(child, TRUE);
            g_string_append(json, ", ");
        }
        g_string_truncate(json, json->len - 2 ); // removes the last ','
        g_string_append(json, "],");
    }

    if(prog != NULL)
    {
        g_string_append(json, "\"Parse_Tree\": {\"Program\": ");
        GString * child = prog_to_json(prog);
        g_string_append(json, child->str);
        g_string_free(child, TRUE);
        g_string_append_c(json, '}');
    }
    g_string_append_c(json, '}');

    fputs(json->str, out);
    fflush(out);
    fclose(out);
    g_string_free(json, TRUE);
    count++;
}

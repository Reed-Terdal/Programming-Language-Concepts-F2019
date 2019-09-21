#include <stdio.h>
#include <stdlib.h>
#include "Scanner/scanner.h"
#include "Parser/parser.h"

void printUsage();

int main(int argc, char ** argv)
{
    if (argc < 2)
    {
        fprintf(stderr, "Missing Jott Source file.\n");
        printUsage();
        return -1;
    }
    GArray * tokenStream = ScanFile(argv[1]);


    g_array_free(tokenStream, TRUE);
    return 0;
}



void printUsage()
{
    printf("Usage: ./jot <path-to-jott-program>");
}
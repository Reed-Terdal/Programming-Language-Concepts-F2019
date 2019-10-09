//
// Created by reedt on 9/17/2019.
//

#include <stdio.h>
#include "parser.h"
#include "parserData.h"
#include "Token.h"

program * ParseTokenStream(GArray * tokenStream)
{
    unsigned long end_index = 0;
    program * retVal = create_program(tokenStream, 0, &end_index);
    if(end_index < tokenStream->len)
    {
        fprintf(stderr, "Syntax Error, not all Tokens consumed");
        exit(-1);
    }
    return retVal;
}

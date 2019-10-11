//
// Created by rterdal on 10/10/19.
//

#ifndef JOTT_INTERPRETTER_ERRORS_H
#define JOTT_INTERPRETTER_ERRORS_H

#include <stdio.h>
#include "i_expr.h"
#include "ids.h"
#include "Token.h"

typedef enum scannerState scannerState;

void type_error(GString * expected, Type actual, GArray * token_stream, unsigned long start);

void undeclared_error(GString * id, GArray * token_stream, unsigned long start);

void unexpected_token_error(GString * expected, tokenType actual, GArray * token_stream, unsigned long start);

void divide_by_zero_int_error(i_expr * lhs, i_expr * rhs);

void trailing_comma_error(GArray * token_stream, unsigned long start);

GString * getBrokenStatement(GArray * token_stream, unsigned long start);

#endif //JOTT_INTERPRETTER_ERRORS_H

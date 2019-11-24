/**************************************************************************************************
*
* File Name: Errors.h
*
* Author: Reed Terdal
*
* Created for CS344 on: 10/10/2019
*
* Purpose: Header that exposes common error printing utilities.
 * If an error occurs at any point the interpreter will try to print an appropriate message using these methods as helpers.
*
**************************************************************************************************/

#ifndef JOTT_INTERPRETTER_ERRORS_H
#define JOTT_INTERPRETTER_ERRORS_H

#include <stdio.h>
#include "i_expr.h"
#include "ids.h"
#include "Token.h"


typedef struct d_expr d_expr;
typedef struct i_expr i_expr;

/**
 * @brief This prints a message about a violation to the type system including the expected types, received type,
 * and the location in the program that the violation occurred.
 * @param expected A string that describes what types were expected.
 * @param actual The type that was received.
 * @param token_stream The token stream of the whole program.
 * @param start The index in the token stream where the violation occurred.
 * @note This function exits with an error code of -1 on completion.
 */
void type_error(GString * expected, Type actual, GArray * token_stream, unsigned long start);

/**
 * @brief This prints a message about the usage of an undeclared indentifier along with contextual information.
 * @param id The name of the undeclared indentifier.
 * @param token_stream The token stream of the whole program.
 * @param start The index in the token stream where the indentifier was used.
 * @note This function exits with an error code of -1 on completion.
 */
void undeclared_error(GString * id, GArray * token_stream, unsigned long start);

/**
 * @brief This prints a message about a violation to the Jott syntax due to an unexpected Token.
 * @param expected A string that describes what types of tokens were expected.
 * @param actual The type of token that was received.
 * @param token_stream The token stream of the whole program.
 * @param start The index in the token stream where the violation occurred.
 * @note This function exits with an error code of -1 on completion.
 */
void unexpected_token_error(GString * expected, tokenType actual, GArray * token_stream, unsigned long start);

/**
 * @brief This prints a message that a runtime error occurred because an integer expression would result in a divide by zero.
 * @param lhs The left-hand integer expression. (numerator)
 * @param rhs The right-hand integer expression. (denominator)
 * @note This function exits with an error code of -1 on completions.
 */
void divide_by_zero_int_error(i_expr * lhs, i_expr * rhs);

/**
 * @brief This prints a message that a runtime error occurred because an double expression would result in a divide by zero.
 * @param lhs The left-hand double expression. (numerator)
 * @param rhs The right-hand double expression. (denominator)
 * @note This function exits with an error code of -1 on completions.
 */
void divide_by_zero_double_error(d_expr * lhs, d_expr * rhs);

/**
 * @brief This prints a message that a trailing comma was found in a function call.
 * @param token_stream The token stream of the whole program.
 * @param start The index in the token stream where the violation occurred.
 * @note This function exits with an error code of -1 on completion.
 */
void trailing_comma_error(GArray * token_stream, unsigned long start);

/**
 * @brief This function creates a string for the statement that caused the violation.
 * @param token_stream The token stream of the whole program.
 * @param start The index in the token stream where the violation occured
 * @return A string of the statement that caused a violation.
 * @note This will start at the provided index and go backwards until the end of the previous statement
 * (; character) or beginning of the program is reached, then it includes every token until it reaches the next end
 * of the current statement (; character) or the end of the program.
 */
GString * getBrokenStatement(GArray * token_stream, unsigned long start);

#endif //JOTT_INTERPRETTER_ERRORS_H

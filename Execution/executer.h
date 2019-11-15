/**************************************************************************************************
*
* File Name: executer.h
*
* Author: Reed Terdal
*
* Created for CS344 on: 10/09/2019
*
* Purpose: Header for the execution phase, exposes the execute function that takes the parse tree and evaluates it.
*
**************************************************************************************************/

#ifndef JOTT_INTERPRETTER_EXECUTER_H
#define JOTT_INTERPRETTER_EXECUTER_H

#include "program.h"

/**
 * @name execute
 * @brief Takes in a parse-tree and executes the program that it represents
 * @param parse_tree The program to be executed
 */
void execute(stmt_list *);

typedef struct string_meta string_meta;

string_meta *evaluate_string_expression(s_expr *sExpr);

void *evaluate_function(f_call *fCall);

void *evaluate_expression(expr *expression);

void *evaluate_assignment(asmt *asmt);

void evaluate_re_assignment(r_asmt *asmt);

void run_for_loop(for_node *forNode);

void run_while_loop(while_node *whileNode);

void run_if_block(if_node *ifNode);

void execute_b_stmt_list(b_stmt_list *stmts);

gdouble evaluate_double_expression(d_expr *dExpr);

gint64 evaluate_int_expression(i_expr *iExpr);

#endif //JOTT_INTERPRETTER_EXECUTER_H

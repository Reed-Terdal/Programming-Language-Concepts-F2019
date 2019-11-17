/**************************************************************************************************
*
* File Name: executer.c
*
* Author: Reed Terdal
*
* Created for CS344 on: 10/09/2019
*
* Purpose: Traverses the parse tree generated by the Parsing phase and executes statements sequentially.
 * This is what actually "runs" Jott code and will print out any results (including runtime-errors).
*
**************************************************************************************************/


#include "executer.h"
#include <math.h>
#include <stdio.h>
#include <Errors.h>


typedef struct string_meta string_meta;
string_meta * evaluate_string_expression(s_expr * sExpr);
void * evaluate_function(f_call * fCall);
void * evaluate_expression(expr * expression);
gdouble evaluate_double_expression(d_expr * dExpr);
gint64 evaluate_int_expression(i_expr * iExpr);
void eval_r_asmt(r_asmt *rAsmt);
void eval_if(if_node *if_stmt);
void eval_for(for_node *for_stmt);
void eval_while(while_node *while_stmt);

/**
 * @details This helps to keep track of "temporary" strings, such as printing out the result of a concat or charAt,
 * this are not assigned at any point and are thrown out after evaluation.
 * This lets us know that we can free this when we are done and not worry about double-frees
 */
typedef struct string_meta
{
    GString * data;
    bool is_intermediate;
}string_meta;

void execute(program * parse_tree)
{
    for(stmt_list * current = parse_tree->statement_list; current != NULL; current = current->statement_list)
    {
        if(current->statement != NULL)
        {
            if(current->statement->expression != NULL)
            {
                void * retval = evaluate_expression(current->statement->expression);
                if(retval != NULL)
                {
                    if(current->statement->expression->string_expression != NULL && ((string_meta *)retval)->is_intermediate)
                    {
                        g_string_free(((string_meta *)retval)->data, TRUE);
                    }
                    free(retval);
                }
            }
            else if(current->statement->assignment != NULL)
            {
                void * retval = evaluate_expression(current->statement->assignment->expression);
                if(current->statement->assignment->id->type == jstring)
                {
                    setGlobalVariable(current->statement->assignment->id->id, ((string_meta *)retval)->data);
                    free(retval);
                }
                else
                {
                    setGlobalVariable(current->statement->assignment->id->id, retval);
                }
            }
            else if(current->statement->function_call != NULL)
            {
                void * retval = evaluate_function(current->statement->function_call);
                if(retval != NULL)
                {
                    if(current->statement->function_call->id->type == jf_str && ((string_meta *)retval)->is_intermediate)
                    {
                        g_string_free(((string_meta *)retval)->data, TRUE);
                    }
                    free(retval);
                }
            }
            else if(current->statement->re_asmt!= NULL){
                eval_r_asmt(current->statement->re_asmt);
            }
            else if(current->statement->ifNode!= NULL){
                eval_if(current->statement->ifNode);
            }
            else if(current->statement->forNode!= NULL){
                eval_for(current->statement->forNode);
            }
            else if(current->statement->whileNode!= NULL){
                eval_while(current->statement->whileNode);
            }
        }
    }
    destroyGlobalScope();
}


/**
 * @brief This is responsible for evaluating function calls
 * @param fCall The fCall that is going to be evaluated
 * @return This will return what ever the id type of the function is ie
 *          jf_int->     int64 *,
 *          jf_double -> double *,
 *          jf_string -> string_meta *,
 *          jf_void -> NULL
 */
void * evaluate_function(f_call * fCall)
{
    if(fCall != NULL)
    {
        if(fCall->id != NULL)
        {
            if(g_str_equal(fCall->id->id->str, "print"))
            {
                if(fCall->params != NULL && fCall->params->expression != NULL)
                {
                    if(fCall->params->expression->int_expression != NULL)
                    {
                        printf("%ld\n", evaluate_int_expression(fCall->params->expression->int_expression));
                    }
                    else if(fCall->params->expression->double_expression != NULL)
                    {
                        printf("%1.9g\n", evaluate_double_expression(fCall->params->expression->double_expression));
                    }
                    else if(fCall->params->expression->string_expression != NULL)
                    {
                        string_meta * result = evaluate_string_expression(fCall->params->expression->string_expression);
                        printf("%s\n", result->data->str);
                        if(result->is_intermediate)
                        {
                            g_string_free(result->data, TRUE);
                        }
                        free(result);
                    }
                }
            }
            else if(g_str_equal(fCall->id->id->str, "concat"))
            {
                if(fCall->params != NULL && fCall->params->expression != NULL &&
                fCall->params->expression->string_expression != NULL &&
                fCall->params->next != NULL && fCall->params->next->expression != NULL &&
                fCall->params->next->expression->string_expression != NULL)
                {
                    string_meta * first = evaluate_string_expression(fCall->params->expression->string_expression);
                    string_meta * second = evaluate_string_expression(fCall->params->next->expression->string_expression);
                    if(first->is_intermediate)
                    {
                        // If it is an intermediate (temporary), we can freely modify it
                        g_string_append(first->data, second->data->str);
                    }
                    else
                    {
                        // This is not an intermediate, meaning something else references it, so we can override with our new temporary
                        GString * new = g_string_new(first->data->str);
                        g_string_append(new, second->data->str);
                        first->data = new;
                    }
                    first->is_intermediate = TRUE; // This was created from other strings, it does not correspond to a permanent location (yet)
                    if(second->is_intermediate)
                    {
                        g_string_free(second->data, TRUE);
                    }
                    free(second);
                    return first;
                }
            }
            else if(g_str_equal(fCall->id->id->str, "charAt"))
            {
                if(fCall->params != NULL && fCall->params->expression != NULL &&
                   fCall->params->expression->string_expression != NULL &&
                   fCall->params->next != NULL && fCall->params->next->expression != NULL &&
                   fCall->params->next->expression->int_expression != NULL)
                {
                    gint64 index = evaluate_int_expression(fCall->params->next->expression->int_expression);
                    string_meta * str = evaluate_string_expression(fCall->params->expression->string_expression);
                    if(index < str->data->len)
                    {
                        GString * new = g_string_new(NULL);
                        g_string_append_c(new, str->data->str[index]);
                        string_meta * final = calloc(1, sizeof(string_meta));
                        final->data = new;
                        final->is_intermediate = TRUE;  // This was created from other strings, it does not correspond to a permanent location (yet)
                        if(str->is_intermediate)
                        {
                            g_string_free(str->data, TRUE);
                        }
                        free(str);
                        return final;
                    }
                }
            }
        }
    }
    return NULL;
}

/**
 * @brief Evaluates an int expression and returns the result
 * @param iExpr The int expression to evaluate
 * @return The result of the evaluation of the int expression
 */
gint64 evaluate_int_expression(i_expr * iExpr)
{
    if(iExpr != NULL)
    {
        if(iExpr->literal != NULL)
        {
            return iExpr->literal->value;
        }
        else if(iExpr->id != NULL)
        {
            runtime_variable * value = getGlobalVariable(iExpr->id->id);
            if(value != NULL && value->value != NULL)
            {
                return *((gint64 *)value->value);
            }
        }
        else if(iExpr->function_call != NULL)
        {
            gint64 * retVal = evaluate_function(iExpr->function_call);
            if(retVal != NULL)
            {
                return *retVal;
            }
        }
        else if(iExpr->LHS_expr != NULL && iExpr->operatorNode != NULL && iExpr->RHS_expr != NULL)
        {
            gint64 lhs = evaluate_int_expression(iExpr->LHS_expr);
            gint64 rhs = evaluate_int_expression(iExpr->RHS_expr);
            switch (iExpr->operatorNode->opType)
            {
                case op_add:  return lhs + rhs;
                case op_sub:  return lhs - rhs;
                case op_mult: return lhs * rhs;
                case op_pow:  return (gint64) pow(lhs, rhs);
                case op_div:
                    if(rhs != 0)
                    {
                        return lhs / rhs;
                    }
                    else
                    {
                        divide_by_zero_int_error(iExpr->LHS_expr, iExpr->RHS_expr);
                    }
                    break;
                case op_greater: return lhs > rhs;
                case op_goe: return lhs >= rhs;
                case op_less: return lhs < rhs;
                case op_loe: return lhs <= rhs;
                case op_eq: return lhs == rhs;
                case op_neq: return lhs != rhs;
            }
        }
        else if(iExpr->LHS_s_expr != NULL && iExpr->RHS_s_expr!= NULL && iExpr->operatorNode!= NULL){
            string_meta *lhs = evaluate_string_expression(iExpr->LHS_s_expr);
            string_meta *rhs = evaluate_string_expression(iExpr->RHS_s_expr);
            int result= strcmp(lhs->data->str, rhs->data->str);
            switch(iExpr->operatorNode->opType){
                case op_greater: return result > 0;
                case op_less: return result < 0;
                case op_goe: return result >= 0;
                case op_loe: return result <= 0;
                case op_eq: return result ==0;
                case op_neq: return result != 0;
            }
        }
        else if(iExpr->LHS_d_expr != NULL && iExpr->RHS_d_expr!= NULL && iExpr->operatorNode!= NULL){
            gdouble lhs = evaluate_double_expression(iExpr->LHS_d_expr);
            gdouble rhs = evaluate_double_expression(iExpr->RHS_d_expr);
            switch(iExpr->operatorNode->opType){
                case op_greater: return lhs > rhs;
                case op_goe: return lhs >= rhs;
                case op_less: return lhs < rhs;
                case op_loe: return lhs <= rhs;
                case op_eq: return lhs == rhs;
                case op_neq: return lhs != rhs;
            }
        }
    }
    return 0;
}

/**
 * @brief Evaluates a string expression and returns the result
 * @param sExpr The string expression to evaluate
 * @return a string_meta which contains the string result and whether or not the string is a temporary
 */
string_meta * evaluate_string_expression(s_expr * sExpr)
{
    if(sExpr != NULL)
    {
        if(sExpr->literal != NULL)
        {
            string_meta * final = calloc(1, sizeof(string_meta));
            final->data = g_string_new(sExpr->literal->value->str);
            final->is_intermediate = TRUE;
            return final;
        }
        else if(sExpr->id != NULL)
        {
            runtime_variable * variable = getGlobalVariable(sExpr->id->id);
            if(variable != NULL && variable->value != NULL)
            {
                string_meta * final = calloc(1, sizeof(string_meta));
                final->data = (GString *)variable->value;
                final->is_intermediate = FALSE;
                return final;
            }
        }
        else if(sExpr->function_call != NULL)
        {
            return (string_meta *)evaluate_function(sExpr->function_call);
        }
    }
    return NULL;
}

/**
 * @brief Evaluates a double expression and returns the result
 * @param dExpr The double expression to evaluate
 * @return the result of the double expression evaluation
 */
gdouble evaluate_double_expression(d_expr * dExpr)
{
    if(dExpr != NULL)
    {
        if(dExpr->double_literal != NULL)
        {
            return dExpr->double_literal->value;
        }
        else if(dExpr->id != NULL)
        {
            runtime_variable * variable = getGlobalVariable(dExpr->id->id);
            if(variable != NULL && variable->value != NULL)
            {
                return *((gdouble *)variable->value);
            }
        }
        else if(dExpr->function_call != NULL)
        {
            gdouble * retval = evaluate_function(dExpr->function_call);
            if(retval != NULL)
            {
                return *retval;
            }
        }
        else if(dExpr->LHS_expr != NULL && dExpr->operator != NULL && dExpr->RHS_expr != NULL)
        {
            gdouble lhs = evaluate_double_expression(dExpr->LHS_expr);
            gdouble rhs = evaluate_double_expression(dExpr->RHS_expr);
            switch (dExpr->operator->opType)
            {
                case op_add:
                    return lhs + rhs;
                case op_sub:
                    return lhs - rhs;
                case op_mult:
                    return lhs - rhs;
                case op_div:
                    if(rhs != 0)
                    {
                        return lhs / rhs;
                    }
                    else
                    {
                        divide_by_zero_double_error(dExpr->LHS_expr, dExpr->RHS_expr);
                    }
                    break;
                case op_pow:
                    return (gdouble) pow(lhs, rhs);
                case op_greater: return lhs > rhs;
                case op_goe: return lhs >= rhs;
                case op_less: return lhs < rhs;
                case op_loe: return lhs <= rhs;
                case op_eq: return lhs == rhs;
                case op_neq: return lhs != rhs;
            }
        }
    }
}

/**
 * @brief Evaluates an expression and returns the result of the evaluation
 * @param expression The expression to evaluate
 * @return A pointer to the result of the evaluation, which depends on the type of expression,
 *      string_expression -> string_meta *
 *      int_expression -> int64 *
 *      double_expression -> double *
 */
void * evaluate_expression(expr * expression)
{
    void * retval = NULL;
    if(expression != NULL)
    {
        if(expression->string_expression != NULL)
        {
            retval = evaluate_string_expression(expression->string_expression);
        }
        else if(expression->int_expression != NULL)
        {
            retval = calloc(1, sizeof(gint64));
            *((gint64 *)retval) = evaluate_int_expression(expression->int_expression);
        }
        else if(expression->double_expression != NULL)
        {
            retval = calloc(1, sizeof(gdouble));
            *((gdouble *)retval) = evaluate_double_expression(expression->double_expression);
        }
    }
    return retval;
}

void eval_r_asmt(r_asmt *rAsmt){
    if(rAsmt!= NULL){
        Type type;
        gboolean hasType= findIDType(rAsmt->id->id, &type);
        if(hasType== FALSE){
            fprintf(stderr, "Identifier has not been detected!");
        }
        else{
            if(rAsmt->expression->double_expression== NULL && type == jdouble){
                fprintf(stderr, "Expression does not match ID type: jdouble\n");
            }
            else if(rAsmt->expression->int_expression== NULL && type== jint){
                fprintf(stderr, "Expression does not match ID type: jint\n");
            }
            else if(rAsmt->expression->string_expression== NULL && type== jstring){
                fprintf(stderr, "Expression does not match ID type: jstring\n");
            }
            else{
                void * retval = evaluate_expression(rAsmt->expression);
                if(rAsmt->id->type == jstring)
                {
                    setGlobalVariable(rAsmt->id->id, ((string_meta *)retval)->data);
                    free(retval);
                }
                else
                {
                    setGlobalVariable(rAsmt->id->id, retval);
                }
            }
        }
    }
}

void eval_b_stmt_list(b_stmt_list *bStmt ){
    for(b_stmt_list * current = bStmt; current != NULL; current = current->next)
    {
        if(current->b_statement != NULL)
        {
            if(current->b_statement->expression != NULL)
            {
                void * retval = evaluate_expression(current->b_statement->expression);
                if(retval != NULL)
                {
                    if(current->b_statement->expression->string_expression != NULL && ((string_meta *)retval)->is_intermediate)
                    {
                        g_string_free(((string_meta *)retval)->data, TRUE);
                    }
                    free(retval);
                }
            }
            else if(current->b_statement->functionCall != NULL)
            {
                void * retval = evaluate_function(current->b_statement->functionCall);
                if(retval != NULL)
                {
                    if(current->b_statement->functionCall->id->type == jf_str && ((string_meta *)retval)->is_intermediate)
                    {
                        g_string_free(((string_meta *)retval)->data, TRUE);
                    }
                    free(retval);
                }
            }
            else if(current->b_statement->reassign!= NULL){
                eval_r_asmt(current->b_statement->reassign);
            }
            else if(current->b_statement->ifBlock!= NULL){
                eval_if(current->b_statement->ifBlock);
            }
            else if(current->b_statement->forLoop!= NULL){
                eval_for(current->b_statement->forLoop);
            }
            else if(current->b_statement->whileLoop!= NULL){
                eval_while(current->b_statement->whileLoop);
            }
        }
    }
    destroyGlobalScope();
}

void eval_if(if_node *if_stmt) {
    if(if_stmt != NULL){
        if(evaluate_int_expression(if_stmt->expression)!= 0){
            eval_b_stmt_list(if_stmt->b_true);
        }
        else{
            if(if_stmt->b_false!= NULL){
                eval_b_stmt_list(if_stmt->b_false);
            }
        }
    }
}

void eval_for(for_node *for_stmt){
    if(for_stmt != NULL){
        void * retval = evaluate_expression(for_stmt->initialize->expression);
        if(for_stmt->initialize->type == jstring)
        {
            setGlobalVariable(for_stmt->initialize->id->id, ((string_meta *)retval)->data);
            free(retval);
        }
        else
        {
            setGlobalVariable(for_stmt->initialize->id->id, retval);
        }
        while(evaluate_int_expression(for_stmt->conditional)!= 0){
            eval_b_stmt_list(for_stmt->body);
            eval_r_asmt(for_stmt->incrementer);
        }
    }
}

void eval_while(while_node *while_stmt){
    if(while_stmt!= NULL){
        while(evaluate_int_expression(while_stmt->conditional)!= 0){
            eval_b_stmt_list(while_stmt->body);
        }
    }
}

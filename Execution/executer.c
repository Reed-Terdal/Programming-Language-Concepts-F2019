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
#include "b_stmt_list.h"
#include "if_node.h"
#include "for_node.h"
#include "while_node.h"


typedef struct string_meta string_meta;
string_meta * evaluate_string_expression(s_expr * sExpr);
void * evaluate_function(f_call * fCall);
void * evaluate_expression(expr * expression);
gdouble evaluate_double_expression(d_expr * dExpr);
gint64 evaluate_int_expression(i_expr * iExpr);
void eval_if(if_node *);
void eval_for(for_node *);
void eval_while(while_node *);
void eval_b_stmt_list(b_stmt_list *);
void eval_assign(asmt *);
void eval_reassign(r_asmt *);


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
                eval_assign(current->statement->assignment);
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
            else if(current->statement->ifBlock != NULL)
            {
                eval_if(current->statement->ifBlock);
            }
            else if(current->statement->forLoop != NULL)
            {
                eval_for(current->statement->forLoop);
            }
            else if(current->statement->whileLoop != NULL)
            {
                eval_while(current->statement->whileLoop);
            }
            else if(current->statement->reassign != NULL)
            {
                eval_reassign(current->statement->reassign);
            }
            else
            {
                fprintf(stderr, "Runtime Error: invalid statement\n");
                exit(-1);
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
            if(iExpr->LHS_expr->double_expression != NULL)
            {
                gdouble lhs = evaluate_double_expression(iExpr->LHS_expr->double_expression);
                gdouble rhs = evaluate_double_expression(iExpr->RHS_expr->double_expression);
                switch (iExpr->operatorNode->opType)
                {
                    case op_comp_less:
                        return lhs < rhs;
                    case op_comp_loe:
                        return lhs <= rhs;
                    case op_comp_eq:
                        return lhs == rhs;
                    case op_comp_neq:
                        return lhs != rhs;
                    case op_comp_goe:
                        return lhs >= rhs;
                    case op_comp_greater:
                        return lhs > rhs;
                    default:
                        fprintf(stderr, "Bad double operation in i_expr\n");
                        exit(-1);
                }
            }
            else if(iExpr->LHS_expr->string_expression != NULL)
            {
                string_meta * lhs = evaluate_string_expression(iExpr->LHS_expr->string_expression);
                string_meta * rhs = evaluate_string_expression(iExpr->RHS_expr->string_expression);
                gint64 result = strcmp(lhs->data->str, rhs->data->str);
                switch (iExpr->operatorNode->opType)
                {
                    case op_comp_less:
                        return result < 0;
                    case op_comp_loe:
                        return result <= 0;
                    case op_comp_eq:
                        return result == 0;
                    case op_comp_neq:
                        return result != 0;
                    case op_comp_goe:
                        return result >= 0;
                    case op_comp_greater:
                        return result > 0;
                    default:
                        fprintf(stderr, "Bad string operation in i_expr\n");
                        exit(-1);
                }
            }
            else if(iExpr->LHS_expr->int_expression != NULL)
            {
                gint64 lhs = evaluate_int_expression(iExpr->LHS_expr->int_expression);
                gint64 rhs = evaluate_int_expression(iExpr->RHS_expr->int_expression);
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
                            divide_by_zero_int_error(iExpr->LHS_expr->int_expression, iExpr->RHS_expr->int_expression);
                        }
                        break;
                    case op_comp_less:
                        return lhs < rhs;
                    case op_comp_loe:
                        return lhs <= rhs;
                    case op_comp_eq:
                        return lhs == rhs;
                    case op_comp_neq:
                        return lhs != rhs;
                    case op_comp_goe:
                        return lhs >= rhs;
                    case op_comp_greater:
                        return lhs > rhs;
                }
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
                case op_comp_less:
                    return lhs < rhs;
                case op_comp_loe:
                    return lhs <= rhs;
                case op_comp_eq:
                    return lhs == rhs;
                case op_comp_neq:
                    return lhs != rhs;
                case op_comp_goe:
                    return lhs >= rhs;
                case op_comp_greater:
                    return lhs > rhs;
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

void eval_if(if_node * ifNode)
{
    if(ifNode != NULL)
    {
        gint64 result = 0;
        if(ifNode->conditional != NULL)
        {
            result = evaluate_int_expression(ifNode->conditional);
            if(result != 0)
            {
                // Condition was true, execute the first block
                if(ifNode->true_branch != NULL)
                {
                    eval_b_stmt_list(ifNode->true_branch);
                }
            }
            else
            {
                if(ifNode->false_branch != NULL)
                {
                    eval_b_stmt_list(ifNode->false_branch);
                }
            }
        }
    }
}

void eval_for(for_node * forNode)
{
    eval_assign(forNode->initializer);

    gint64 result = evaluate_int_expression(forNode->conditional);
    while(result != 0)
    {
        eval_b_stmt_list(forNode->body);
        eval_reassign(forNode->reassign);
        result = evaluate_int_expression(forNode->conditional);
    }
}

void eval_while(while_node * whileNode)
{
    if(whileNode != NULL)
    {
        gint64 result = evaluate_int_expression(whileNode->conditional);
        while(result != 0)
        {
            eval_b_stmt_list(whileNode->body);
            result = evaluate_int_expression(whileNode->conditional);
        }
    }
}

void eval_b_stmt_list(b_stmt_list * bStmtList)
{
    for(b_stmt_list * current = bStmtList; current != NULL; current = current->next)
    {
        if(current->bStmt != NULL)
        {
            if(current->bStmt->expression != NULL)
            {
                void * retval = evaluate_expression(current->bStmt->expression);
                if(retval != NULL)
                {
                    if(current->bStmt->expression->string_expression != NULL && ((string_meta *)retval)->is_intermediate)
                    {
                        g_string_free(((string_meta *)retval)->data, TRUE);
                    }
                    free(retval);
                }
            }
            else if(current->bStmt->functionCall != NULL)
            {
                void * retval = evaluate_function(current->bStmt->functionCall);
                if(retval != NULL)
                {
                    if(current->bStmt->functionCall->id->type == jf_str && ((string_meta *)retval)->is_intermediate)
                    {
                        g_string_free(((string_meta *)retval)->data, TRUE);
                    }
                    free(retval);
                }
            }
            else if(current->bStmt->ifBlock != NULL)
            {
                eval_if(current->bStmt->ifBlock);
            }
            else if(current->bStmt->forLoop != NULL)
            {
                eval_for(current->bStmt->forLoop);
            }
            else if(current->bStmt->whileLoop != NULL)
            {
                eval_while(current->bStmt->whileLoop);
            }
            else if(current->bStmt->reassign != NULL)
            {
                eval_reassign(current->bStmt->reassign);
            }
            else
            {
                fprintf(stderr, "Runtime Error: invalid statement\n");
                exit(-1);
            }
        }
    }
}

void eval_assign(asmt * assignment)
{
    void * retval = evaluate_expression(assignment->expression);
    if(assignment->id->type == jstring)
    {
        setGlobalVariable(assignment->id->id, ((string_meta *)retval)->data);
        free(retval);
    }
    else
    {
        setGlobalVariable(assignment->id->id, retval);
    }
}

void eval_reassign(r_asmt * rAsmt)
{
    void * retval = evaluate_expression(rAsmt->expression);
    Type idType;

    if(!findIDType(rAsmt->id->id, &idType))
    {
        fprintf(stderr, "Runtime Error: Tried to reassign an undeclared variable\n");
        exit(-1);
    }

    if(idType == jstring)
    {
        setGlobalVariable(rAsmt->id->id, ((string_meta *)retval)->data);
        free(retval);
    }
    else
    {
        setGlobalVariable(rAsmt->id->id, retval);
    }

}
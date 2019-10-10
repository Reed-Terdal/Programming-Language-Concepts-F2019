//
// Created by reedt on 10/9/2019.
//

#include "semantic_analyzer.h"
#include <math.h>
#include <stdio.h>

void * evaluate_function(f_call * fCall);
void * evaluate_expression(expr * expression);
gdouble evaluate_double_expression(d_expr * dExpr);
GString * evaluate_string_expression(s_expr * sExpr);
gint64 evaluate_int_expression(i_expr * iExpr);



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
                    if(current->statement->expression->string_expression != NULL)
                    {
                        g_string_free(retval, TRUE);
                    }
                    else
                    {
                        free(retval);
                    }
                }
            }
            else if(current->statement->assignment != NULL)
            {
                GString * id_copy = g_string_new(current->statement->assignment->id->id->str);
                setGlobalVariable(id_copy, evaluate_expression(current->statement->assignment->expression));
            }
            else if(current->statement->function_call != NULL)
            {
                void * retval = evaluate_function(current->statement->function_call);
                if(retval != NULL)
                {
                    Type check;
                    if(findIDType(current->statement->function_call->id->id, &check) && check == jf_str)
                    {
                        g_string_free(retval, TRUE);
                    }
                    else
                    {
                        free(retval);
                    }
                }
            }
        }
    }
    destroyGlobalScope();
}


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
                        printf("%1.1f\n", evaluate_double_expression(fCall->params->expression->double_expression));
                    }
                    else if(fCall->params->expression->string_expression != NULL)
                    {
                        printf("%s\n", evaluate_string_expression(fCall->params->expression->string_expression)->str);
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
                    GString * first = g_string_new(evaluate_string_expression(fCall->params->expression->string_expression)->str);
                    GString * second = evaluate_string_expression(fCall->params->next->expression->string_expression);
                    g_string_append(first, second->str);
                    g_string_free(second, TRUE);
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
                    GString * str = evaluate_string_expression(fCall->params->expression->string_expression);
                    if(index < str->len)
                    {
                        GString * new = g_string_new(NULL);
                        g_string_append_c(new, str->str[index]);
                        return new;
                    }
                }
            }
        }
    }
    return NULL;
}

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
                    break;
            }
        }
    }
    return 0;
}

GString * evaluate_string_expression(s_expr * sExpr)
{
    if(sExpr != NULL)
    {
        if(sExpr->literal != NULL)
        {
            GString * copy = g_string_new(sExpr->literal->value->str);
            return copy;
        }
        else if(sExpr->id != NULL)
        {
            runtime_variable * variable = getGlobalVariable(sExpr->id->id);
            if(variable != NULL && variable->value != NULL)
            {
                return (GString *)variable->value;
            }
        }
        else if(sExpr->function_call != NULL)
        {
            return (GString *)evaluate_function(sExpr->function_call);
        }
    }
    return NULL;
}

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
                    break;
                case op_pow:
                    return (gdouble) pow(lhs, rhs);
            }
        }
    }
    return 0.0;
}

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

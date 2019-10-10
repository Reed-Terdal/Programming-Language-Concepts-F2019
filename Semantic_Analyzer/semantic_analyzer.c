//
// Created by reedt on 10/9/2019.
//

#include "semantic_analyzer.h"
#include <math.h>
#include <stdio.h>

typedef struct string_meta
{
    GString * data;
    bool is_intermediate;
}string_meta;

void * evaluate_function(f_call * fCall);
void * evaluate_expression(expr * expression);
gdouble evaluate_double_expression(d_expr * dExpr);
string_meta * evaluate_string_expression(s_expr * sExpr);
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
                        g_string_append(first->data, second->data->str);
                    }
                    else
                    {
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

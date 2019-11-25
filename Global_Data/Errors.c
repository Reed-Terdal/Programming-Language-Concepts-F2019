/**************************************************************************************************
*
* File Name: Errors.c
*
* Author: Reed Terdal
*
* Created for CS344 on: 10/10/2019
*
* Purpose: Provides helpers for common error printing utilities.
 * If an error occurs at any point the interpreter will try to print an appropriate message using these methods.
*
**************************************************************************************************/

#include "Errors.h"
#include "d_expr.h"
#include "i_expr.h"

GString * i_expr_to_str(i_expr * iExpr);
GString * d_expr_to_str(d_expr * dExpr);

void type_error(GString * expected, Type actual, GArray * token_stream, unsigned long start)
{
    fprintf(stderr, "Syntax Error: Type mismatch: Expected %s got ", expected->str);
    switch (actual)
    {
        case jf_void:
            fprintf(stderr, "'Void Function', \"");
            break;
        case jf_str:
            fprintf(stderr, "'String Function', \"");
            break;
        case jf_double:
            fprintf(stderr, "'Double Function', \"");
            break;
        case jf_int:
            fprintf(stderr, "'Integer Function', \"");
            break;
        case jint:
            fprintf(stderr, "Integer, \"");
            break;
        case jdouble:
            fprintf(stderr, "Double, \"");
            break;
        case jstring:
            fprintf(stderr, "String, \"");
            break;
        default:
            fprintf(stderr, "UNKNOWN, \"");
    }
    fprintf(stderr, "%s", getBrokenStatement(token_stream, start)->str);
    fprintf(stderr, "\" (%s:%d:%d)\n", __filePath, g_array_index(token_stream, Token, start).line_num, g_array_index(token_stream, Token, start).col_num);
    exit(-1);
}

void undeclared_error(GString * id, GArray * token_stream, unsigned long start)
{
    fprintf(stderr, "Syntax Error: Use of Undeclared ID(\"%s\") in expression, ", id->str);
    fprintf(stderr, "\"%s\"", getBrokenStatement(token_stream, start)->str);
    fprintf(stderr, " (%s:%d:%d)\n", __filePath, g_array_index(token_stream, Token, start).line_num, g_array_index(token_stream, Token, start).col_num);
    exit(-1);
}

void unexpected_token_error(GString * expected, tokenType actual, GArray * token_stream, unsigned long start)
{
    fprintf(stderr, "Syntax Error: Unexpected Token: Expected %s got ", expected->str);
    switch (actual)
    {
        case t_plus:
            fprintf(stderr, "\'+\', ");
            break;
        case t_minus:
            fprintf(stderr, "\'-\', ");
            break;
        case t_multiply:
            fprintf(stderr, "\'*\', ");
            break;
        case t_divide:
            fprintf(stderr, "\'/\', ");
            break;
        case t_power:
            fprintf(stderr, "\'^\', ");
            break;
        case t_end_paren:
            fprintf(stderr, "\')\', ");
            break;
        case t_start_paren:
            fprintf(stderr, "\'(\', ");
            break;
        case t_end_stmt:
            fprintf(stderr, "\';\', ");
            break;
        case t_assign:
            fprintf(stderr, "\'=\', ");
            break;
        case t_integer:
            fprintf(stderr, "\'Integer Literal\', ");
            break;
        case t_floating:
            fprintf(stderr, "\'Double Literal\', ");
            break;
        case t_id:
            fprintf(stderr, "\'Identifier\', ");
            break;
        case t_string:
            fprintf(stderr, "\'String Literal\', ");
            break;
        case t_comma:
            fprintf(stderr, "\',\', ");
            break;
        case t_type_double:
            fprintf(stderr, "\'Double Type\', ");
            break;
        case t_type_integer:
            fprintf(stderr, "\'Integer Type\', ");
            break;
        case t_type_string:
            fprintf(stderr, "\'String Type\', ");
            break;
        default:
            fprintf(stderr, "\'UNKNOWN\', ");
    }
    fprintf(stderr, "\"%s\"", getBrokenStatement(token_stream, start)->str);
    fprintf(stderr, " (%s:%d:%d)\n", __filePath, g_array_index(token_stream, Token, start).line_num, g_array_index(token_stream, Token, start).col_num);
    exit(-1);
}

void divide_by_zero_int_error(i_expr * lhs, i_expr * rhs)
{
    fprintf(stderr, "Runtime Error: Divide by zero, \"%s / %s\"", i_expr_to_str(lhs)->str, i_expr_to_str(rhs)->str);
    fprintf(stderr, " (%s:%lu)\n", __filePath, 0ul);
    exit(-1);
}

GString * i_expr_to_str(i_expr * iExpr)
{
    GString * retval = g_string_new(NULL);

    if(iExpr != NULL)
    {
        if(iExpr->id != NULL)
        {
            g_string_append(retval, iExpr->id->id->str);
        }
        else if(iExpr->literal != NULL)
        {
            g_string_printf(retval, "%ld", iExpr->literal->value);
        }
        else if(iExpr->LHS_expr != NULL && iExpr->operatorNode != NULL && iExpr->RHS_expr != NULL)
        {
            g_string_append(retval, i_expr_to_str(iExpr->LHS_expr->int_expression)->str);
            switch (iExpr->operatorNode->opType)
            {
                case op_add:
                    g_string_append(retval, " + ");
                    break;
                case op_sub:
                    g_string_append(retval, " - ");
                    break;
                case op_mult:
                    g_string_append(retval, " * ");
                    break;
                case op_div:
                    g_string_append(retval, " / ");
                    break;
                case op_pow:
                    g_string_append(retval, " ^ ");
                    break;
                case op_comp_less:
                    g_string_append(retval, " < ");
                    break;
                case op_comp_loe:
                    g_string_append(retval, " <= ");
                    break;
                case op_comp_eq:
                    g_string_append(retval, " == ");
                    break;
                case op_comp_neq:
                    g_string_append(retval, " != ");
                    break;
                case op_comp_goe:
                    g_string_append(retval, " >= ");
                    break;
                case op_comp_greater:
                    g_string_append(retval, " > ");
                    break;
            }
            g_string_append(retval, i_expr_to_str(iExpr->RHS_expr->int_expression)->str);
        }
    }

    return retval;
}

void divide_by_zero_double_error(d_expr * lhs, d_expr * rhs)
{
    fprintf(stderr, "Runtime Error: Divide by zero, \"%s / %s\"", d_expr_to_str(lhs)->str, d_expr_to_str(rhs)->str);
    fprintf(stderr, " (%s:%lu)\n", __filePath, 0ul);
    exit(-1);
}

GString * d_expr_to_str(d_expr * dExpr)
{
    GString * retval = g_string_new(NULL);

    if(dExpr != NULL)
    {
        if(dExpr->id != NULL)
        {
            g_string_append(retval, dExpr->id->id->str);
        }
        else if(dExpr->double_literal != NULL)
        {
            g_string_printf(retval, "%1.9g", dExpr->double_literal->value);
        }
        else if(dExpr->LHS_expr != NULL && dExpr->operator != NULL && dExpr->RHS_expr != NULL)
        {
            g_string_append(retval, d_expr_to_str(dExpr->LHS_expr)->str);
            switch (dExpr->operator->opType)
            {
                case op_add:
                    g_string_append(retval, " + ");
                    break;
                case op_sub:
                    g_string_append(retval, " - ");
                    break;
                case op_mult:
                    g_string_append(retval, " * ");
                    break;
                case op_div:
                    g_string_append(retval, " / ");
                    break;
                case op_pow:
                    g_string_append(retval, " ^ ");
                    break;
                case op_comp_less:
                    g_string_append(retval, " < ");
                    break;
                case op_comp_loe:
                    g_string_append(retval, " <= ");
                    break;
                case op_comp_eq:
                    g_string_append(retval, " == ");
                    break;
                case op_comp_neq:
                    g_string_append(retval, " != ");
                    break;
                case op_comp_goe:
                    g_string_append(retval, " >= ");
                    break;
                case op_comp_greater:
                    g_string_append(retval, " > ");
                    break;
            }
            g_string_append(retval, d_expr_to_str(dExpr->RHS_expr)->str);
        }
    }

    return retval;
}

void trailing_comma_error(GArray * token_stream, unsigned long start)
{
    fprintf(stderr, "Syntax Error: Expected ( Expression ), got ',' ");
    fprintf(stderr, "%s", getBrokenStatement(token_stream, start)->str);
    fprintf(stderr, "\" (%s:%d:%d)\n", __filePath, g_array_index(token_stream, Token, start).line_num, g_array_index(token_stream, Token, start).col_num);
    exit(-1);
}

GString * getBrokenStatement(GArray * token_stream, unsigned long start)
{
    GString * retval = g_string_new(NULL);
    unsigned long start_location = start;
    while(start_location > 0 && g_array_index(token_stream, Token, start_location).type != t_end_stmt)
    {
        /// Finds the beginning of the statement or file
        start_location--;
    }
    if(start_location != 0)
    {
        start_location++;
    }
    for(unsigned long end_location = start_location; end_location < token_stream->len; end_location++)
    {
        /// Append until we get the the end of the statement or file
        g_string_append_printf(retval,"%s", g_array_index(token_stream, Token, end_location).data->str);
        if(g_array_index(token_stream, Token, end_location).type == t_end_stmt)
        {
            break;
        }
    }
    return retval;
}

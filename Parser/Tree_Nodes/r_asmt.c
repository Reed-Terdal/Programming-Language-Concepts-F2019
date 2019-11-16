/**************************************************************************************************
*
* File Name: r_asmt.c
*
* Author: Jameson Toper
*
* Created for CS344 on: 11/08/2019
*
* Purpose: code that provides functions for manipulating r_asmt nodes.
*
**************************************************************************************************/


#include <stdio.h>
#include "r_asmt.h"

r_asmt *create_r_asmt(GArray *tokenStream, unsigned long index, unsigned long *next) {
    /**
     * Token[index] is ID
     * Token[index+1] is '='
     * Token[index+2 <--> index+n] is expression, need to confirm result is same type
     * Token[index+n+1] is ';'
     */

    r_asmt *new_asmt = calloc(1, sizeof(r_asmt));

    //Get expression so we know the type
    // Get the variable in the reassignment
    Token *curToken = &g_array_index(tokenStream, Token, (*next));
    // Hold the type of the ID, will compare later
    Type t;
    // Does the ID exist, because we cant reassign a non existant assignment
    if (!findIDType(curToken->data, &t)) {
        // We tried to redefine a variable that doesnt exit
        fprintf(stderr, "Syntax Error: Incorrect reassignment");
        exit(-1);
    }
    new_asmt->id = create_id_node(curToken);
    (*next)++;
    curToken = &g_array_index(tokenStream, Token, (*next));
    if (curToken->type != t_assign) {
        fprintf(stderr, "Syntax Error: Unexpected Token in reassignment");
        exit(-1);
    }
    (*next)++;
    new_asmt->expression = create_expr(tokenStream, (*next), next);

    if (t == jdouble || t == jf_double) {
        if (new_asmt->expression->double_expression == NULL) {
            fprintf(stderr, "Syntax Error: Incorrect reassignment, types do not match.");
            exit(-1);
        }
    }
    if (t == jint || t == jf_int) {
        if (new_asmt->expression->int_expression == NULL) {
            fprintf(stderr, "Syntax Error: Incorrect reassignment, types do not match.");
            exit(-1);
        }
    }
    if (t == jstring || t == jf_str) {
        if (new_asmt->expression->string_expression == NULL) {
            fprintf(stderr, "Syntax Error: Incorrect reassignment, types do not match.");
            exit(-1);
        }
    }

    return new_asmt;
}

/**
 * Genereate json dump of a r_asmt node
 * @param assignment - the reassignment node to convert
 * @return GString with Json representation of the r_asmt node
 */
GString *r_asmt_to_json(r_asmt *assignment) {
    GString *retVal = g_string_new(NULL);
    if (assignment != NULL) {
        // Print the name of the value to reassign
        g_string_append(retVal, "{\"Reassignment\": ");
        if (assignment->id != NULL) {
            GString *Child = id_node_to_json(assignment->id);
            g_string_append(retVal, Child->str);
            g_string_free(Child, TRUE);
        } else {
            g_string_append(retVal, "null");
        }

        // Print the expression that it will be set to
        g_string_append(retVal, ", \"Expression\": ");
        if (assignment->expression != NULL) {
            GString *Child = expr_to_json(assignment->expression);
            g_string_append(retVal, Child->str);
            g_string_free(Child, TRUE);
        } else {
            g_string_append(retVal, "null");
        }
        g_string_append_c(retVal, '}');
    } else {
        g_string_append(retVal, "null");
    }
    return retVal;
}

void destroy_r_asmt(r_asmt *assignment) {
    if (assignment != NULL) {
        if (assignment->expression != NULL) {
            destroy_expr(assignment->expression);
        }
        if (assignment->id != NULL) {
            destroy_id_node(assignment->id);
        }
        free(assignment);
    }
}
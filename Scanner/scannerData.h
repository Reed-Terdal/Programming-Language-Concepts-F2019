/**************************************************************************************************
*
* File Name: scannerData.h
*
* Author: Reed Terdal
*
* Created for CS344 on: 09/17/2019
*
* Purpose: Data needed for the scanner.
*
**************************************************************************************************/


#ifndef JOTT_INTERPRETTER_SCANNERDATA_H
#define JOTT_INTERPRETTER_SCANNERDATA_H

#include "Token.h"

/**
 * This enum represents the different states that the Jott scanner can be in
 */
typedef enum scannerState
{
    qERR = 0, // having this be 0 makes our list initialization much easier
    q0, // Empty
    q1, // Id or keyword
    q2, // period
    q3, // number (floating)
    q4, // number (integral)
    q5, // = (assignment)
    q6, // start paren
    q7, // end paren
    q8, // end_stmt
    q9, // plus
    q10, // minus
    q11, // multiply
    q12, // divide
    q13, // power
    q14, // start quote
    q15, // end quote
    q16, // comma
    q17, // COMMENT
    q18, // less <
    q19, // less equal <=
    q20, // greater >
    q21, // greater equal >=
    q22, // not !
    q23, // not equal
    q24, // equal to ==
    q25, // start bracket {
    q26 // end bracket }
} scannerState;

scannerState current_state;

scannerState previous_state;

#define FINAL_ACCEPT_STATE [0 ... 255]=q0
/**
 * This Look-Up-Table represents the DFA for tokenizing a Jott program.
 * The first index is the current scanner state and the second index is the current character being processed.
 * If transitioning from !q0 -> q0 we accept the token. qERR is a bad state if we get incorrect input.
 */
scannerState state_LUT[28][256] = {
        /* qERR*/ {},//Default initialized, qERR only leads to qERR
        /* q0  */ {['A'...'Z']=q1, ['a'...'z']=q1, ['.']=q2, ['0'...'9']=q3, ['=']=q5, ['(']=q6, [')']=q7, [';']=q8, ['+']=q9, ['-']=q10, ['*']=q11, ['/']=q12, ['^']=q13, ['"']=q14, [',']=q16, ['\r']=q0, ['\n']=q0, [' ']=q0, ['\t']=q0, ['<']=q18, ['>']=q20, ['!']=q22, ['{']=q25, ['}']=q26},
        /* q1  */ {['A'...'Z']=q1, ['a'...'z']=q1,['0'...'9']=q1,   [0 ... '/']=q0, [':'...'@']=q0, ['['...'`']=q0, ['{'...255]=q0},
        /* q2  */ {['0'...'9']=q4,                                  [0 ... '/']=q0, [':'... 255]=q0},
        /* q3  */ {['0'...'9']=q3, ['.']=q4,                        [0 ... '-']=q0, ['/']=q0, [':'... 255]=q0},
        /* q4  */ {['0'...'9']=q4,                                  [0 ... '/']=q0, [':'... 255]=q0},
        /* q5  */ {['=']=q24,                                       [0 ... '<']=q0, ['>' ... 255]=q0},
        /* q6  */ {FINAL_ACCEPT_STATE},
        /* q7  */ {FINAL_ACCEPT_STATE},
        /* q8  */ {FINAL_ACCEPT_STATE},
        /* q9  */ {FINAL_ACCEPT_STATE},
        /* q10 */ {FINAL_ACCEPT_STATE},
        /* q11 */ {FINAL_ACCEPT_STATE},
        /* q12 */ {['/'] = q17,                                     [0 ...'.']=q0, ['0'...255]=q0},
        /* q13 */ {FINAL_ACCEPT_STATE},
        /* q14 */ {['A'...'Z']=q14, ['a'...'z']=q14, ['0'...'9']=q14, [' ']=q14, ['"']=q15},
        /* q15 */ {FINAL_ACCEPT_STATE},
        /* q16 */ {FINAL_ACCEPT_STATE},
        /* q17 */ {[0 ... 9]=q17, [11 ... 255]=q17,                 ['\n']=q0},
        /* q18 */ {['=']=q19,                                       [0 ... '<']=q0, ['>' ... 255]=q0},
        /* q19 */ {FINAL_ACCEPT_STATE},
        /* q20 */ {['=']=q21,                                       [0 ... '<']=q0, ['>' ... 255]=q0},
        /* q21 */ {FINAL_ACCEPT_STATE},
        /* q22 */ {['=']=q23},
        /* q23 */ {FINAL_ACCEPT_STATE},
        /* q24 */ {FINAL_ACCEPT_STATE},
        /* q25 */ {FINAL_ACCEPT_STATE},
        /* q26 */ {FINAL_ACCEPT_STATE},
};

/**
 * This Look-Up-Table represents what token is being built based on the current scanner state.
 * Index is current scanner state
 */
tokenType tokenType_LUT[28] = {
        /*qERR*/ t_INVALID,
        /*q0  */ t_INVALID,
        /*q1  */ t_id,
        /*q2  */ t_INVALID,
        /*q3  */ t_integer,
        /*q4  */ t_floating,
        /*q5  */ t_assign,
        /*q6  */ t_start_paren,
        /*q7  */ t_end_paren,
        /*q8  */ t_end_stmt,
        /*q9  */ t_plus,
        /*q10 */ t_minus,
        /*q11 */ t_multiply,
        /*q12 */ t_divide,
        /*q13 */ t_power,
        /*q14 */ t_INVALID,
        /*q15 */ t_string,
        /*q16 */ t_comma,
        /*q17 */ t_INVALID,
        /*q18 */ t_comp_less,
        /*q19 */ t_comp_loe,
        /*q20 */ t_comp_greater,
        /*q21 */ t_comp_goe,
        /*q22 */ t_INVALID,
        /*q23 */ t_comp_neq,
        /*q24 */ t_comp_eq,
        /*q25 */ t_start_bracket,
        /*q26 */ t_end_bracket
};





#endif //JOTT_INTERPRETTER_SCANNERDATA_H

//
// Created by rterdal on 9/21/19.
//

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
} scannerState;

scannerState current_state;

#define FINAL_ACCEPT_STATE [0 ... 255]=q0
/**
 * This Look-Up-Table represents the DFA for tokenizing a Jott program.
 * The first index is the current scanner state and the second index is the current character being processed.
 * If transitioning from !q0 -> q0 we accept the token. qERR is a bad state if we get incorrect input.
 */
scannerState state_LUT[18][256] = {
        /* qERR*/ {},//Default initialized, qERR only leads to qERR
        /* q0  */ {['A'...'Z']=q1, ['a'...'z']=q1, ['.']=q2, ['0'...'9']=q3, ['=']=q5, ['(']=q6, [')']=q7, [';']=q8, ['+']=q9, ['-']=q10, ['*']=q11, ['/']=q12, ['^']=q13, ['"']=q14, [',']=q16, ['\r']=q0, ['\n']=q0, [' ']=q0, ['\t']=q0},
        /* q1  */ {['A'...'Z']=q1, ['a'...'z']=q1,['0'...'9']=q1,   [0 ... '/']=q0, [':'...'@']=q0, ['['...'`']=q0, ['{'...255]=q0},
        /* q2  */ {['0'...'9']=q4,                                  [0 ... '/']=q0, [':'... 255]=q0},
        /* q3  */ {['0'...'9']=q3, ['.']=q4,                        [0 ... '-']=q0, ['/']=q0, [':'... 255]=q0},
        /* q4  */ {['0'...'9']=q4,                                  [0 ... '/']=q0, [':'... 255]=q0},
        /* q5  */ {FINAL_ACCEPT_STATE},
        /* q6  */ {FINAL_ACCEPT_STATE},
        /* q7  */ {FINAL_ACCEPT_STATE},
        /* q8  */ {FINAL_ACCEPT_STATE},
        /* q9  */ {FINAL_ACCEPT_STATE},
        /* q10 */ {FINAL_ACCEPT_STATE},
        /* q11 */ {FINAL_ACCEPT_STATE},
        /* q12 */ {FINAL_ACCEPT_STATE},
        /* q13 */ {FINAL_ACCEPT_STATE},
        /* q14 */ {['A'...'Z']=q14, ['a'...'z']=q14, ['0'...'9']=q14, [' ']=q14, ['"']=q15},
        /* q15 */ {FINAL_ACCEPT_STATE},
        /* q16 */ {FINAL_ACCEPT_STATE}
};

/**
 * This Look-Up-Table represents what token is being built based on the current scanner state.
 * Index is current scanner state
 */
tokenType tokenType_LUT[18] = {
        /*qERR*/ INVALID,
        /*q0  */ INVALID,
        /*q1  */ id_or_keyword,
        /*q2  */ INVALID,
        /*q3  */ number,
        /*q4  */ number,
        /*q5  */ assign,
        /*q6  */ start_paren,
        /*q7  */ end_paren,
        /*q8  */ end_stmt,
        /*q9  */ plus,
        /*q10 */ minus,
        /*q11 */ multiply,
        /*q12 */ divide,
        /*q13 */ power,
        /*q14 */ INVALID,
        /*q15 */ string,
        /*q16 */ comma
};





#endif //JOTT_INTERPRETTER_SCANNERDATA_H

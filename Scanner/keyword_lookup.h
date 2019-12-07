/**************************************************************************************************
*
* File Name: keyword_lookup.h
*
* Author: Reed Terdal
*
* Created for CS344 on: 09/26/2019
*
* Purpose: Header that defines a perfect hash table for finding keywords in the Jott language.
* See GPerf for more information
*
**************************************************************************************************/
#ifndef JOTT_INTERPRETTER_KEYWORD_LOOKUP_H
#define JOTT_INTERPRETTER_KEYWORD_LOOKUP_H

/* ANSI-C code produced by gperf version 3.1 */
/* Command-line: gperf -t gperf_generator.txt  */
/* Computed positions: -k'1' */

#if !((' ' == 32) && ('!' == 33) && ('"' == 34) && ('#' == 35) \
      && ('%' == 37) && ('&' == 38) && ('\'' == 39) && ('(' == 40) \
      && (')' == 41) && ('*' == 42) && ('+' == 43) && (',' == 44) \
      && ('-' == 45) && ('.' == 46) && ('/' == 47) && ('0' == 48) \
      && ('1' == 49) && ('2' == 50) && ('3' == 51) && ('4' == 52) \
      && ('5' == 53) && ('6' == 54) && ('7' == 55) && ('8' == 56) \
      && ('9' == 57) && (':' == 58) && (';' == 59) && ('<' == 60) \
      && ('=' == 61) && ('>' == 62) && ('?' == 63) && ('A' == 65) \
      && ('B' == 66) && ('C' == 67) && ('D' == 68) && ('E' == 69) \
      && ('F' == 70) && ('G' == 71) && ('H' == 72) && ('I' == 73) \
      && ('J' == 74) && ('K' == 75) && ('L' == 76) && ('M' == 77) \
      && ('N' == 78) && ('O' == 79) && ('P' == 80) && ('Q' == 81) \
      && ('R' == 82) && ('S' == 83) && ('T' == 84) && ('U' == 85) \
      && ('V' == 86) && ('W' == 87) && ('X' == 88) && ('Y' == 89) \
      && ('Z' == 90) && ('[' == 91) && ('\\' == 92) && (']' == 93) \
      && ('^' == 94) && ('_' == 95) && ('a' == 97) && ('b' == 98) \
      && ('c' == 99) && ('d' == 100) && ('e' == 101) && ('f' == 102) \
      && ('g' == 103) && ('h' == 104) && ('i' == 105) && ('j' == 106) \
      && ('k' == 107) && ('l' == 108) && ('m' == 109) && ('n' == 110) \
      && ('o' == 111) && ('p' == 112) && ('q' == 113) && ('r' == 114) \
      && ('s' == 115) && ('t' == 116) && ('u' == 117) && ('v' == 118) \
      && ('w' == 119) && ('x' == 120) && ('y' == 121) && ('z' == 122) \
      && ('{' == 123) && ('|' == 124) && ('}' == 125) && ('~' == 126))
/* The character set is not based on ISO-646.  */
#error "gperf generated tables don't work with this execution character set. Please report a bug to <bug-gperf@gnu.org>."
#endif

#line 1 "gperf_generator.txt"

#include "Token.h"
#line 4 "gperf_generator.txt"

struct keyword_lookup {char *name; int type;};

#define TOTAL_KEYWORDS 10
#define MIN_WORD_LENGTH 2
#define MAX_WORD_LENGTH 7
#define MIN_HASH_VALUE 2
#define MAX_HASH_VALUE 16
/* maximum key range = 15, duplicates = 0 */

#ifdef __GNUC__
__inline
#else
#ifdef __cplusplus
inline
#endif
#endif
static unsigned int
hash (register const char *str, register size_t len)
{
    static unsigned char asso_values[] =
            {
                    17, 17, 17, 17, 17, 17, 17, 17, 17, 17,
                    17, 17, 17, 17, 17, 17, 17, 17, 17, 17,
                    17, 17, 17, 17, 17, 17, 17, 17, 17, 17,
                    17, 17, 17, 17, 17, 17, 17, 17, 17, 17,
                    17, 17, 17, 17, 17, 17, 17, 17, 17, 17,
                    17, 17, 17, 17, 17, 17, 17, 17, 17, 17,
                    17, 17, 17, 17, 17, 17, 17, 17, 10, 17,
                    17, 17, 17,  0, 17, 17, 17, 17, 17, 17,
                    17, 17, 17,  5, 17, 17, 10, 17, 17, 17,
                    17, 17, 17, 17, 17, 17, 17, 17, 17, 17,
                    17,  5,  0, 17, 17,  0, 17, 17, 17, 17,
                    17, 17, 17, 17,  0, 17, 17, 17,  0,  0,
                    17, 17, 17, 17, 17, 17, 17, 17, 17, 17,
                    17, 17, 17, 17, 17, 17, 17, 17, 17, 17,
                    17, 17, 17, 17, 17, 17, 17, 17, 17, 17,
                    17, 17, 17, 17, 17, 17, 17, 17, 17, 17,
                    17, 17, 17, 17, 17, 17, 17, 17, 17, 17,
                    17, 17, 17, 17, 17, 17, 17, 17, 17, 17,
                    17, 17, 17, 17, 17, 17, 17, 17, 17, 17,
                    17, 17, 17, 17, 17, 17, 17, 17, 17, 17,
                    17, 17, 17, 17, 17, 17, 17, 17, 17, 17,
                    17, 17, 17, 17, 17, 17, 17, 17, 17, 17,
                    17, 17, 17, 17, 17, 17, 17, 17, 17, 17,
                    17, 17, 17, 17, 17, 17, 17, 17, 17, 17,
                    17, 17, 17, 17, 17, 17, 17, 17, 17, 17,
                    17, 17, 17, 17, 17, 17
            };
    return len + asso_values[(unsigned char)str[0]];
}

struct keyword_lookup *
in_word_set (register const char *str, register size_t len)
{
    static struct keyword_lookup wordlist[] =
            {
                    {""}, {""},
#line 12 "gperf_generator.txt"
                    {"if", t_if},
#line 10 "gperf_generator.txt"
                    {"for", t_for},
#line 14 "gperf_generator.txt"
                    {"void", t_type_void},
#line 11 "gperf_generator.txt"
                    {"while", t_while},
#line 16 "gperf_generator.txt"
                    {"return", t_return},
#line 8 "gperf_generator.txt"
                    {"Integer", t_type_integer},
                    {""},
#line 13 "gperf_generator.txt"
                    {"else", t_else},
                    {""},
#line 9 "gperf_generator.txt"
                    {"String", t_type_string},
                    {""}, {""},
#line 15 "gperf_generator.txt"
                    {"Void", t_type_void},
                    {""},
#line 7 "gperf_generator.txt"
                    {"Double", t_type_double}
            };

    if (len <= MAX_WORD_LENGTH && len >= MIN_WORD_LENGTH)
    {
        register unsigned int key = hash (str, len);

        if (key <= MAX_HASH_VALUE)
        {
            register const char *s = wordlist[key].name;

            if (*str == *s && !strcmp (str + 1, s + 1))
                return &wordlist[key];
        }
    }
    return 0;
}
#endif //JOTT_INTERPRETTER_KEYWORD_LOOKUP_H

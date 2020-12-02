/**
 * Project: Implementation of a compiler of the IFJ20 language.
 * @file    token_types.h
 * @brief   This is the header file with definitions of constants.
 * @author  Robin Stancl <xstanc09@stud.fit.vutbr.cz>
 * @date    12. 10. 2020
 */

/**
 * Definition of constants
 */

#ifndef IFJ20_COMPILER_TOKEN_TYPES_H
#define IFJ20_COMPILER_TOKEN_TYPES_H

#define TOKEN_EMPTY                 1

#define TOKEN_EOF                   100
#define TOKEN_COLON_EQUAL           101
#define TOKEN_PLUS                  102
#define TOKEN_MINUS                 103
#define TOKEN_MULTIPLICATION        104
#define TOKEN_DIVISION              105
#define TOKEN_LEFT_BRACKET          106
#define TOKEN_RIGHT_BRACKET         107
#define TOKEN_LEFT_CURLY_BRACE      108
#define TOKEN_RIGHT_CURLY_BRACE     109
#define TOKEN_COMMA                 110
#define TOKEN_EQUAL                 111
#define TOKEN_UNDERSCORE            112
#define TOKEN_NOT_EQUAL             113
#define TOKEN_LEFT_ARROW            114
#define TOKEN_RIGHT_ARROW           115
#define TOKEN_LESS_OR_EQUAL         116
#define TOKEN_GREATER_OR_EQUAL      117
#define TOKEN_DOUBLE_EQUAL          118
#define TOKEN_STRING_LITERAL        119
#define TOKEN_INTEGER_LITERAL       120
#define TOKEN_DECIMAL_LITERAL       121
#define TOKEN_EXPONENT_LITERAL      122
#define TOKEN_SEMICOLON             123
#define TOKEN_IDENTIFIER            124
#define TOKEN_KEYWORD_FOR           125
#define TOKEN_KEYWORD_FLOAT64       126
#define TOKEN_KEYWORD_ELSE          127
#define TOKEN_KEYWORD_IF            128
#define TOKEN_KEYWORD_FUNC          129
#define TOKEN_KEYWORD_INT           130
#define TOKEN_KEYWORD_PACKAGE       131
#define TOKEN_KEYWORD_RETURN        132
#define TOKEN_KEYWORD_STRING        133
#define TOKEN_FUNCTION_INPUTS       134
#define TOKEN_FUNCTION_INPUTI       135
#define TOKEN_FUNCTION_INPUTF       136
#define TOKEN_FUNCTION_PRINT        137
#define TOKEN_FUNCTION_SUBSTR       138
#define TOKEN_FUNCTION_ORD          139
#define TOKEN_FUNCTION_CHR          140
#define TOKEN_FUNCTION_LEN          141
#define TOKEN_FUNCTION_INT2FLOAT    142
#define TOKEN_FUNCTION_FLOAT2INT    143
#define TOKEN_ZERO_INTEGER_LITERAL  144

#endif //IFJ20_COMPILER_TOKEN_TYPES_H

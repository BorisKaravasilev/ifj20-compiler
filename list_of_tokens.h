/**
 * Project: Implementation of a compiler of the IFJ20 language.
 * @file    list_of_tokens.h
 * @brief   This is the header file with definitions of constants.
 * @author  Robin Stancl <xstanc09@stud.fit.vutbr.cz>
 * @date    12. 10. 2020
 */

/**
 * Definition of constants
 */

#define SCANNER_ERROR               1

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
#define TOKEN_EXPONENT              122
#define TOKEN_IDENTIFIER            123
#define TOKEN_KEY_WORD_FOR          124
#define TOKEN_KEY_WORD_FLOAT64      125
#define TOKEN_KEY_WORD_ELSE         126
#define TOKEN_KEY_WORD_IF           127
#define TOKEN_KEY_WORD_FUNC         128
#define TOKEN_KEY_WORD_INT          129
#define TOKEN_KEY_WORD_PACKAGE      130
#define TOKEN_KEY_WORD_RETURN       131
#define TOKEN_KEY_WORD_STRING       132
#define TOKEN_FUNCTION_INPUTS       133
#define TOKEN_FUNCTION_INPUTI       134
#define TOKEN_FUNCTION_INPUTF       135
#define TOKEN_FUNCTION_PRINT        136
#define TOKEN_FUNCTION_SUBSTR       137
#define TOKEN_FUNCTION_ORD          138
#define TOKEN_FUNCTION_CHR          139
#define TOKEN_FUNCTION_LEN          140
#define TOKEN_FUNCTION_INT2FLOAT    141
#define TOKEN_FUNCTION_FLOAT2INT    142
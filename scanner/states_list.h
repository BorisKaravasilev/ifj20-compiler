/**
 * Project: Implementation of a compiler of the IFJ20 language.
 * @file    states_list.h
 * @brief   This is the header file with definitions of states constats.
 * @author  Robin Stancl <xstanc09@stud.fit.vutbr.cz>
 * @date    21. 10. 2020
 */

#ifndef IFJ20_COMPILER_STATES_LIST_H
#define IFJ20_COMPILER_STATES_LIST_H

#define STATE_START                         200
#define STATE_EOF                           201
#define STATE_MINUS                         202
#define STATE_PLUS                          203
#define STATE_MULTIPLICATION                204
#define STATE_DIVISION                      205
#define STATE_LEFT_BRACKET                  206
#define STATE_RIGHT_BRACKET                 207
#define STATE_COMMA                         208
#define STATE_COLON                         209
#define STATE_COLON_EQUAL                   210
#define STATE_EXPLANATION_MARK              211
#define STATE_NOT_EQUAL                     212
#define STATE_LEFT_ARROW                    213
#define STATE_LESS_OR_EQUAL                 214
#define STATE_RIGHT_ARROW                   215
#define STATE_GREATER_OR_EQUAL              216
#define STATE_LEFT_CURLY_BRACE              217
#define STATE_RIGHT_CURLY_BRACE             218
#define STATE_SEMICOLON                     219
#define STATE_EQUAL                         220
#define STATE_DOUBLE_EQUAL                  221
#define STATE_LINE_COMMENT_BODY             222
#define STATE_LINE_COMMENT                  223
#define STATE_BLOCK_COMMENT_BODY            224
#define STATE_BLOCK_COMMENT_END             225
#define STATE_BLOCK_COMMENT                 226
#define STATE_STRING                        227
#define STATE_BACKSLASH                     228
#define STATE_F                             229
#define STATE_FF                            230
#define STATE_HEX_NUMBER                    231
#define STATE_STRING_LITERAL                232
#define STATE_UNDERSCORE                    233
#define STATE_IDENTIFIER                    234
#define STATE_INTEGER_LITERAL               235
#define STATE_DECIMAL_NUMBER_NOT_END        236
#define STATE_DECIMAL_LITERAL               237
#define STATE_EXPONENT                      238
#define STATE_PLUS_MINUS                    239
#define STATE_EXPONENT_LITERAL              240
#define STATE_SLASH                         241

#endif //IFJ20_COMPILER_STATES_LIST_H

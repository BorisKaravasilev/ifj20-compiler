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
#define STATE_COLON                         201
#define STATE_EXPLANATION_MARK              202
#define STATE_LINE_COMMENT_BODY             203
#define STATE_BLOCK_COMMENT_BODY            204
#define STATE_BLOCK_COMMENT_END             205
#define STATE_STRING                        206
#define STATE_BACKSLASH                     207
#define STATE_F                             208
#define STATE_FF                            209
#define STATE_HEX_NUMBER                    210
#define STATE_DECIMAL_NUMBER_NOT_END        211
#define STATE_EXPONENT                      212
#define STATE_PLUS_MINUS                    213

#endif //IFJ20_COMPILER_STATES_LIST_H

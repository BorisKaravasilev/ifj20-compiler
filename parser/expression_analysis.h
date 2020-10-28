/**
 * Project: Implementation of a compiler of the IFJ20 language.
 * @file    expression_analysis.h
 * @brief   This is the header file with heads of functions and definitions of constants.
 * @author  Robin Stancl <xstanc09@stud.fit.vutbr.cz>
 * @date    28. 10. 2020
 */

#ifndef IFJ20_COMPILER_EXPRESSION_ANALYSIS_H
#define IFJ20_COMPILER_EXPRESSION_ANALYSIS_H

#include "../scanner/token_functions.h"

/**
 * Heads of functions
 */

///The parsing function for the expressions
///If an expression starts with the identifier token, *ptr_identifier_token parameter contains this token
/// otherwise it contains NULL
///Parameter *ptr_start_token contains the first token of an expression
/// (or second one if the expression starts with the identifier token)
///In the parameter *ptr_last_token, this function returns the first token after an expression (The expr_check function
/// must read one token after an expression ends to recognize if the whole expression has been read)
///In the parameter *ptr_expr_data_type, the expr_check function also returns a type of the whole expression.
/// To do so it uses definitions of constants below (EXPRESSION_INT, EXPRESSION_FLOAT64, EXPRESSION_STRING)
///The function returns 0 if everything is fine, or RC_SYN_ERR constant defined in "../general/return_codes.h"
/// if an error occurred
int expr_check(tokenT *ptr_identifier_token, tokenT *ptr_start_token, tokenT *ptr_last_token, int *ptr_expr_data_type);

/**
 * Definitions of constants
 */

#define EXPRESSION_INT          40
#define EXPRESSION_FLOAT64      41
#define EXPRESSION_STRING       42

#endif //IFJ20_COMPILER_EXPRESSION_ANALYSIS_H

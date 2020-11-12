/**
 * Project: Implementation of a compiler of the IFJ20 language.
 * @file    expression_analysis.h
 * @brief   This is the header file with heads of functions and definitions of constants.
 * @author  Robin Stancl <xstanc09@stud.fit.vutbr.cz>
 * @date    28. 10. 2020
 */

#ifndef IFJ20_COMPILER_EXPRESSION_ANALYSIS_H
#define IFJ20_COMPILER_EXPRESSION_ANALYSIS_H

#include "scanner_functions.h"
#include "token_functions.h"

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
///In the parameter *ptr_expr_data_and_type, the expr_check function also returns a data type of the whole expression.
/// To do so it uses definitions of constants below (EXPRESSION_INT, EXPRESSION_FLOAT64, EXPRESSION_STRING,
/// EXPRESSION_BOOL_TRUE, EXPRESSION_BOOL_FALSE, EXPRESSION_NO_TYPE)
/// tokenT.token_type = data type of the expression
/// The result of the expression is saved in tokenT.attribute.string_val
///The function returns 0 if everything is fine, or RC_SYN_ERR, RC_SEMANTIC_IDENTIFIER_ERR, RC_ZERO_DIVISION_ERR
/// or RC_RUN_ERR or RC_SEMANTIC_TYPE_COMPATIBILITY_ER
/// or RC_SEMANTIC_VAR_TYPE_ERR  constant defined in "../general/return_codes.h"
/// if an error occurred
int expr_check(tokenT *ptr_identifier_token, tokenT *ptr_start_token, tokenT *ptr_last_token, tokenT *ptr_expr_data_and_type, scannerT *scanner);
///The function for check if the token is an operator -,*,<,>,<=,>=
int operator_check(tokenT *ptr_tok_num);

/**
 * Definitions of constants
 */

#define EXPRESSION_INT          40
#define EXPRESSION_FLOAT64      41
#define EXPRESSION_STRING       42
#define EXPRESSION_BOOL_TRUE    43
#define EXPRESSION_BOOL_FALSE   44
#define EXPRESSION_NO_TYPE      45

/**
 * Definition of stack and variables
 */

///The variable for saving the the number of saved token
int number_of_token;
///The variable for moving in the switch
int switch_case;
///The number of brackets (++, )--
int number_of_brackets;
///Variable for value returned by semantic analysis
int semantic_data_type;
///Variable for saving if it is the first token of expression
/// If the variable is set to 0 it is first token, else it is not first
int is_it_first_token_of_expression;

#endif //IFJ20_COMPILER_EXPRESSION_ANALYSIS_H

/**
 * Project: Implementation of a compiler of the IFJ20 language.
 * @file    expression_generator.h
 * @brief   This is the header file with heads of functions and definitions of constants.
 * @author  Robin Stancl <xstanc09@stud.fit.vutbr.cz>
 */

#ifndef IFJ20_COMPILER_EXPRESSION_GENERATOR_H
#define IFJ20_COMPILER_EXPRESSION_GENERATOR_H

#include "token_linked_list.h"
#include "generator.h"
#include "token_types.h"
#include "string_functions.h"
#include "return_codes.h"

#define EXPR_RESULT_HERE        "TF@result_here"


///Function for generating the IFJcode20 for the expression
///The token_list_generate parameter needs list of tokens of the expression in the postfix notation
int generate_expression(token_listT *token_list_generate, int division_type);

#endif //IFJ20_COMPILER_EXPRESSION_GENERATOR_H

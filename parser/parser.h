/**
 * Project: Implementation of a compiler of the IFJ20 language.
 *
 * @file parser.h
 * @brief Parser header
 * @author Dominik Vecera <xvecer23@stud.fit.vutbr.cz>
 * @author Petr Vrtal <xvrtal01@stud.fit.vutbr.cz>
 */

#ifndef IFJ20_COMPILER_PARSER_H
#define IFJ20_COMPILER_PARSER_H

#include <stdio.h>

#include "expression_analysis.h"
#include "return_codes.h"
#include "scanner_functions.h"
#include "token_types.h"
#include "semantic_functions.h"
#include "semantic_late_check.h"

#define SYNTAX_OK 0
#define LEFT_SIDE_TOKEN_COUNT 50

/*
 * Print error string to stderr
 */
void err_print(char *str, int token_type);

/*
 * An identifier - regular or special _
 */
int id(scannerT *ptr_scanner, tokenT token[], bool assign_allowed);

/*
 * An item - integer, float or string
 */
int literal(scannerT *ptr_scanner, tokenT token[], int *item_type);

/*
 * An item - integer, float or string
 */
int item(scannerT *ptr_scanner, tokenT token[], int *item_type);

/*
 * The optional continuation of a print statement
 */
int print_next(scannerT *ptr_scanner, tokenT token[]);

/*
 * The beginning of a print statement
 */
int print(scannerT *ptr_scanner, tokenT token[]);

/*
 * A built-in function
 */
int builtin_func(scannerT *ptr_scanner, tokenT token[], int *built_in_func_type);

/*
 * An expression - resolved by precedential syntactic analysis
 */
int expr(scannerT *ptr_scanner, tokenT token[], bool two_tokens, int *result_data_type);

/*
 * An individual assignment
 */
int assign(scannerT *ptr_scanner, tokenT token[], bool *skip_sides_semantic_type_check);

/*
 * The optional continuation of a list of assignments
 */
int assign_next(scannerT *ptr_scanner, tokenT token[], bool *skip_sides_semantic_type_check);

/*
 * The beginning of a list of assignments
 */
int assign_list(scannerT *ptr_scanner, tokenT token[], bool *skip_sides_semantic_type_check);

/*
 * An individual assignment (without function calls)
 */
int assign_nofunc(scannerT *ptr_scanner, tokenT token[]);

/*
 * The optional continuation of a list of assignments (without function calls)
 */
int assign_nofunc_next(scannerT *ptr_scanner, tokenT token[], int return_value_number);

/*
 * The beginning of a list of assignments (without function calls)
 */
int assign_nofunc_list(scannerT *ptr_scanner, tokenT token[]);

/*
 * The optional continuation of a list of identifiers
 */
int id_next1(scannerT *ptr_scanner, tokenT token[], int param_num, bool assignment);

/*
 * The beginning of a list of identifiers
 */
int id_list1(scannerT *ptr_scanner, tokenT token[], bool assignment);

/*
 * The optional continuation of a list of identifiers
 */
int id_next2(scannerT *ptr_scanner, tokenT token[], int id_number);

/*
 * The beginning of a list of identifiers
 */
int id_list2(scannerT *ptr_scanner, tokenT token[]);

/*
 * A command beginning with an underscore (one or multiple assignments)
 */
int underscore_command(scannerT *ptr_scanner, tokenT token[]);

/*
 * A command with an identifier (definition, one or multiple assignments, function call)
 */
int id_command(scannerT *ptr_scanner, tokenT token[]);

/*
 * An optional assignment at the end of each cycle iteration
 */
int cycle_assign(scannerT *ptr_scanner, tokenT token[], int for_count);

/*
 * Optional initial definition in a cycle
 */
int cycle_init(scannerT *ptr_scanner, tokenT token[]);

/*
 * An individual command inside a function, cycle, or block
 */
int command(scannerT *ptr_scanner, tokenT token[]);

/*
 * The list of commands inside a function, cycle, or block
 */
int command_list(scannerT *ptr_scanner, tokenT token[]);

/*
 * The optional continuation of a list of function return types
 */
int return_type(scannerT *ptr_scanner, tokenT token[], ST_Item *ptr_curr_symbol, int return_type_num);

/*
 * The beginning of a list of function return types
 */
int return_type_list(scannerT *ptr_scanner, tokenT token[], ST_Item *ptr_curr_symbol);

/*
 * The optional continuation of a list of function parameters
 */
int param(scannerT *ptr_scanner, tokenT token[], ST_Item *ptr_curr_symbol, int param_number);

/*
 * The beginning of a list of function parameters
 */
int param_list(scannerT *ptr_scanner, tokenT token[], ST_Item *ptr_curr_symbol);

/*
 * A function definition
 */
int func(scannerT *ptr_scanner, tokenT token[]);

/*
 * The program itself - a list of functions
 */
int program(scannerT *ptr_scanner, tokenT token[]);

/*
 * Starting point of parser
 */
int parse(scannerT *ptr_scanner, tokenT token[]);

#endif  // IFJ20_COMPILER_PARSER_H

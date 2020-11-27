/**
 * Project: Implementation of a compiler of the IFJ20 language.
 *
 * @file parser.h
 * @brief Parser header
 * @author Dominik Večeřa <xvecer23@stud.fit.vutbr.cz>
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

void err_print(char *str, int token_type);

int id(scannerT *ptr_scanner, tokenT token[], bool assign_allowed);
int literal(scannerT *ptr_scanner, tokenT token[], int *item_type);
int item(scannerT *ptr_scanner, tokenT token[], int *item_type);
int print_next(scannerT *ptr_scanner, tokenT token[]);
int print(scannerT *ptr_scanner, tokenT token[]);
int builtin_func(scannerT *ptr_scanner, tokenT token[], int *built_in_func_type);
int expr(scannerT *ptr_scanner, tokenT token[], bool two_tokens, int *result_data_type);
int assign(scannerT *ptr_scanner, tokenT token[], bool *skip_sides_semantic_type_check);
int assign_next(scannerT *ptr_scanner, tokenT token[], bool *skip_sides_semantic_type_check);
int assign_list(scannerT *ptr_scanner, tokenT token[], bool *skip_sides_semantic_type_check);
int assign_nofunc(scannerT *ptr_scanner, tokenT token[]);
int assign_nofunc_next(scannerT *ptr_scanner, tokenT token[]);
int assign_nofunc_list(scannerT *ptr_scanner, tokenT token[]);
int id_next1(scannerT *ptr_scanner, tokenT token[]);
int id_list1(scannerT *ptr_scanner, tokenT token[]);
int id_next2(scannerT *ptr_scanner, tokenT token[]);
int id_list2(scannerT *ptr_scanner, tokenT token[]);
int id_command(scannerT *ptr_scanner, tokenT token[]);
int cycle_assign(scannerT *ptr_scanner, tokenT token[]);
int cycle_init(scannerT *ptr_scanner, tokenT token[]);
int command(scannerT *ptr_scanner, tokenT token[]);
int command_list(scannerT *ptr_scanner, tokenT token[]);
int return_type(scannerT *ptr_scanner, tokenT token[], ST_Item *ptr_curr_symbol);
int return_type_list(scannerT *ptr_scanner, tokenT token[], ST_Item *ptr_curr_symbol);
int param(scannerT *ptr_scanner, tokenT token[], ST_Item *ptr_curr_symbol);
int param_list(scannerT *ptr_scanner, tokenT token[], ST_Item *ptr_curr_symbol);
int func(scannerT *ptr_scanner, tokenT token[]);
int program(scannerT *ptr_scanner, tokenT token[]);
int parse(scannerT *ptr_scanner, tokenT token[]);

#endif  // IFJ20_COMPILER_PARSER_H

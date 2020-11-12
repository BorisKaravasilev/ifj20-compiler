/**
 * Project: Implementation of a compiler of the IFJ20 language.
 *
 * @file parser.h
 * @brief Parser header
 * @author Dominik Večeřa <xvecer23@stud.fit.vutbr.cz>
 * @date 11. 11. 2020
 */

#ifndef IFJ20_COMPILER_PARSER_H
#define IFJ20_COMPILER_PARSER_H

#include <stdio.h>

#include "../general/return_codes.h"
#include "../scanner/scanner_functions.h"
#include "../scanner/token_types.h"

#define SYNTAX_OK 0

int item(scannerT *ptr_scanner, tokenT token[]);
int print_next(scannerT *ptr_scanner, tokenT token[]);
int print(scannerT *ptr_scanner, tokenT token[]);
int builtin_func(scannerT *ptr_scanner, tokenT token[]);
int expr(scannerT *ptr_scanner, tokenT token[]);
int assign(scannerT *ptr_scanner, tokenT token[]);
int assign_next(scannerT *ptr_scanner, tokenT token[]);
int assign_list(scannerT *ptr_scanner, tokenT token[]);
int id_next1(scannerT *ptr_scanner, tokenT token[]);
int id_list1(scannerT *ptr_scanner, tokenT token[]);
int id_next2(scannerT *ptr_scanner, tokenT token[]);
int id_list2(scannerT *ptr_scanner, tokenT token[]);
int id_command(scannerT *ptr_scanner, tokenT token[]);
int cycle_assign(scannerT *ptr_scanner, tokenT token[]);
int cycle_init(scannerT *ptr_scanner, tokenT token[]);
int command(scannerT *ptr_scanner, tokenT token[]);
int command_list(scannerT *ptr_scanner, tokenT token[]);
int return_type(scannerT *ptr_scanner, tokenT token[]);
int return_type_list(scannerT *ptr_scanner, tokenT token[]);
int param(scannerT *ptr_scanner, tokenT token[]);
int param_list(scannerT *ptr_scanner, tokenT token[]);
int func(scannerT *ptr_scanner, tokenT token[]);
int program(scannerT *ptr_scanner, tokenT token[]);
int parse(scannerT *ptr_scanner, tokenT token[]);

#endif //IFJ20_COMPILER_PARSER_H

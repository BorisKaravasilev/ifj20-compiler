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

int item(scannerT scanner, tokenT token[]);
int print_next(scannerT scanner, tokenT token[]);
int print(scannerT scanner, tokenT token[]);
int builtin_func(scannerT scanner, tokenT token[]);
int expr(scannerT scanner, tokenT token[]);
int assign(scannerT scanner, tokenT token[]);
int assign_next(scannerT scanner, tokenT token[]);
int assign_list(scannerT scanner, tokenT token[]);
int id_next1(scannerT scanner, tokenT token[]);
int id_list1(scannerT scanner, tokenT token[]);
int id_next2(scannerT scanner, tokenT token[]);
int id_list2(scannerT scanner, tokenT token[]);
int id_command(scannerT scanner, tokenT token[]);
int cycle_assign(scannerT scanner, tokenT token[]);
int cycle_init(scannerT scanner, tokenT token[]);
int command(scannerT scanner, tokenT token[]);
int command_list(scannerT scanner, tokenT token[]);
int return_type(scannerT scanner, tokenT token[]);
int return_type_list(scannerT scanner, tokenT token[]);
int param(scannerT scanner, tokenT token[]);
int param_list(scannerT scanner, tokenT token[]);
int func(scannerT scanner, tokenT token[]);
int program(scannerT scanner, tokenT token[]);
int parse(scannerT scanner, tokenT token[]);

#endif //IFJ20_COMPILER_PARSER_H

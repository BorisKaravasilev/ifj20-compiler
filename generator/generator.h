/**
 * Project: Implementation of a compiler of the IFJ20 language.
 *
 * @file generator.h
 * @brief Functions for generating "IFJcode20"
 * @author Boris Karavasilev <xkarav01@stud.fit.vutbr.cz>
 * @date 21. 11. 2020
 */

#ifndef IFJ20_COMPILER_GENERATOR_H
#define IFJ20_COMPILER_GENERATOR_H

#include "token_functions.h"

#define RETURN_VALUE_1 "TF@retval1"
#define RETURN_VALUE_2 "TF@retval2"

// Code blocks generation
void gen_enter_main();
void gen_exit_main();
void gen_enter_function_scope();
void gen_leave_function_scope();

// Utility functions
void gen_assign_token_to_var(char *var, tokenT *token);
void gen_print(tokenT *token_to_print);

// Built-in functions
void gen_call_input(int func_token_type, tokenT *token_array, int tok_index);
void gen_def_builtin_functions(const bool builtin_func_used[]);
void gen_def_inputs();
void gen_def_inputi();
void gen_def_inputf();

// Prace s ramci, volani funkci
void gen_move(char *var, char *symb);
void gen_move_to_lf(char *var, char *symb);
void gen_move_to_tf(char *var, char *symb);
void gen_createframe();
void gen_pushframe();
void gen_popframe();
void gen_defvar(char *var);
void gen_defvar_lf(char *var);
void gen_defvar_tf(char *var);
void gen_call(char *label);
void gen_return();

// Prace s datovym zasobnikem
void gen_pushs(char *symb);
void gen_pops(char *var);
void gen_clears();

// Aritmeticke, relacni, booleovske a konverzni instrukce
void gen_add(char *var, char *symb1, char *symb2);
void gen_sub(char *var, char *symb1, char *symb2);
void gen_mul(char *var, char *symb1, char *symb2);
void gen_div(char *var, char *symb1, char *symb2);
void gen_idiv(char *var, char *symb1, char *symb2);

void gen_adds();
void gen_subs();
void gen_muls();
void gen_divs();
void gen_idivs();

void gen_lt(char *var, char *symb1, char *symb2);
void gen_gt(char *var, char *symb1, char *symb2);
void gen_eq(char *var, char *symb1, char *symb2);

void gen_lts();
void gen_gts();
void gen_eqs();

void gen_and(char *var, char *symb1, char *symb2);
void gen_or(char *var, char *symb1, char *symb2);
void gen_not(char *var, char *symb);

void gen_ands();
void gen_ors();
void gen_nots();

void gen_int2float(char *var, tokenT *symb_token);
void gen_float2int(char *var, tokenT *symb_token);
void gen_int2char(char *var, char *symb);
void gen_stri2int(char *var, char *symb1, char *symb2);

void gen_int2floats();
void gen_float2ints();
void gen_int2chars();
void gen_stri2ints();

// Vstupne-vystupni instrukce
void gen_read(char *var, char *type);
void gen_write(char *symb);
void gen_concat(char *var, char *symb1, char *symb2);
void gen_strlen(char *var, tokenT *symb_token);
void gen_getchar(char *var, char *symb1, char *symb2);
void gen_setchar(char *var, char *symb1, char *symb2);

// Prace s typy
void gen_type(char *var, char *type);

// Instrukce pro rizeni toku programu
void gen_label(char *label);
void gen_jump(char *label);
void gen_jumpifeq(char *label, char *symb1, char *symb2);
void gen_jumpifneq(char *label, char *symb1, char *symb2);
void gen_jumpifeqs(char *label);
void gen_jumpifneqs(char *label);
void gen_exit(char *symb);

// Ladici instrukce
void gen_break();
void gen_dprint(char *symb);

#endif //IFJ20_COMPILER_GENERATOR_H

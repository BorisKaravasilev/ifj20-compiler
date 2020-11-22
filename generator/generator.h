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

void gen_move(char *var, char *symb);
void gen_createframe();
void gen_pushframe();
void gen_popframe();
void gen_defvar(char *var);
void gen_call(char *label);
void gen_return();
void gen_pushs(char *symb);
void gen_pops(char *var);
void gen_clears();
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
void gen_int2float(char *var, char *symb);
void gen_float2int(char *var, char *symb);
void gen_int2char(char *var, char *symb);
void gen_stri2int(char *var, char *symb1, char *symb2);
void gen_int2floats();
void gen_float2ints();
void gen_int2chars();
void gen_stri2ints();
void gen_read(char *var, char *type);
void gen_write(char *symb);
void gen_concat(char *var, char *symb1, char *symb2);
void gen_strlen(char *var, char *symb);
void gen_getchar(char *var, char *symb1, char *symb2);

#endif //IFJ20_COMPILER_GENERATOR_H

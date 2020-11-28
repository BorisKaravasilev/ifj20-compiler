/**
 * Project: Implementation of a compiler of the IFJ20 language.
 *
 * @file generator.c
 * @brief Functions for generating "IFJcode20"
 * @author Boris Karavasilev <xkarav01@stud.fit.vutbr.cz>
 * @date 21. 11. 2020
 */

#include "generator.h"
#include <stdio.h>

// IFJcode20 converted to C string by:
// https://tomeko.net/online_tools/cpp_text_escape.php?lang=en

void gen_file_start() {
    printf(".IFJcode20\n");
    gen_jump("$$main");

    printf("\n# ----------------------------------- main()\n");
    gen_label("$$main");
}

void gen_file_end() {
    gen_exit("int@0");
}

void gen_enter_function_scope() {
    gen_createframe();
    gen_pushframe();
}

void gen_leave_function_scope() {
    gen_popframe();
}

void gen_def_inputs() {
    printf(
            "# ----------------------------------- inputs()\n"
           "LABEL $inputs\n"
           "PUSHFRAME\n"
           "\n"
           "# init return values\n"
           "DEFVAR LF@retval1\n"
           "MOVE LF@retval1 nil@nil\n"
           "DEFVAR LF@retval2\n"
           "MOVE LF@retval2 nil@nil\n"
           "\n"
           "# function body\n"
           "READ LF@retval1 string\n"
           "JUMPIFNEQ $inputs$else LF@retval1 nil@nil # if (retval1 == nil) retval2 = 1;\n"
           "# error\n"
           "MOVE LF@retval2 int@1\n"
           "JUMP $inputs$endif\n"
           "\n"
           "LABEL $inputs$else # else { retval2 = 0 }\n"
           "# ok\n"
           "MOVE LF@retval2 int@0\n"
           "LABEL $inputs$endif\n"
           "\n"
           "POPFRAME\n"
           "RETURN\n");
}

void gen_def_inputi() {
    printf("# ----------------------------------- inputi()\n"
           "LABEL $inputi\n"
           "PUSHFRAME\n"
           "\n"
           "# init return values\n"
           "DEFVAR LF@retval1\n"
           "MOVE LF@retval1 nil@nil\n"
           "DEFVAR LF@retval2\n"
           "MOVE LF@retval2 nil@nil\n"
           "\n"
           "# function body\n"
           "READ LF@retval1 int\n"
           "JUMPIFNEQ $inputi$else LF@retval1 nil@nil # if (retval1 == nil) retval2 = 1;\n"
           "# error\n"
           "MOVE LF@retval2 int@1\n"
           "JUMP $inputi$endif\n"
           "\n"
           "LABEL $inputi$else # else { retval2 = 0 }\n"
           "# ok\n"
           "MOVE LF@retval2 int@0\n"
           "LABEL $inputi$endif\n"
           "\n"
           "POPFRAME\n"
           "RETURN\n");
}

void gen_def_inputf() {
    printf("# ----------------------------------- inputf()\n"
           "LABEL $inputf\n"
           "PUSHFRAME\n"
           "\n"
           "# init return values\n"
           "DEFVAR LF@retval1\n"
           "MOVE LF@retval1 nil@nil\n"
           "DEFVAR LF@retval2\n"
           "MOVE LF@retval2 nil@nil\n"
           "\n"
           "# function body\n"
           "READ LF@retval1 float\n"
           "JUMPIFNEQ $inputf$else LF@retval1 nil@nil # if (retval1 == nil) retval2 = 1;\n"
           "# error\n"
           "MOVE LF@retval2 int@1\n"
           "JUMP $inputf$endif\n"
           "\n"
           "LABEL $inputf$else # else { retval2 = 0 }\n"
           "# ok\n"
           "MOVE LF@retval2 int@0\n"
           "LABEL $inputf$endif\n"
           "\n"
           "POPFRAME\n"
           "RETURN\n"
           "");
}

void gen_def_print(int number_of_parameters) {
    printf("# ----------------------------------- print()\n"
           "LABEL $print\n"
           "PUSHFRAME\n"
           "\n");

    for (int i = 0; i < number_of_parameters; ++i) {
        printf("WRITE LF@param%d\n", i + 1);
    }

    printf("\n"
           "POPFRAME\n"
           "RETURN");
}

// ---------------------------------------------------| Prace s ramci, volani funkci
void gen_move(char *var, char *symb) {
    printf("MOVE %s %s\n", var, symb);
}

void gen_createframe() {
    printf("CREATEFRAME\n");
}

void gen_pushframe() {
    printf("PUSHFRAME\n");
}
void gen_popframe() {
    printf("POPFRAME\n");
}

void gen_defvar(char *var) {
    printf("DEFVAR %s\n", var);
}

void gen_call(char *label) {
    printf("CALL %s\n", label);
}

void gen_return() {
    printf("RETURN\n");
}

// ---------------------------------------------------| Prace s datovym zasobnikem
void gen_pushs(char *symb) {
    printf("PUSHS %s\n", symb);
}

void gen_pops(char *var) {
    printf("POPS %s\n", var);
}

void gen_clears() {
    printf("CLEARS\n");
}

// ---------------------------------------------------| Aritmeticke, relacni, booleovske a konverzni instrukce
void gen_add(char *var, char *symb1, char *symb2) {
    printf("ADD %s %s %s\n", var, symb1, symb2);
}

void gen_sub(char *var, char *symb1, char *symb2) {
    printf("SUB %s %s %s\n", var, symb1, symb2);
}

void gen_mul(char *var, char *symb1, char *symb2) {
    printf("MUL %s %s %s\n", var, symb1, symb2);
}

void gen_div(char *var, char *symb1, char *symb2) {
    printf("DIV %s %s %s\n", var, symb1, symb2);
}

void gen_idiv(char *var, char *symb1, char *symb2) {
    printf("IDIV %s %s %s\n", var, symb1, symb2);
}

void gen_adds() {
    printf("ADDS\n");
}

void gen_subs() {
    printf("SUBS\n");
}

void gen_muls() {
    printf("MULS\n");
}

void gen_divs() {
    printf("DIVS\n");
}

void gen_idivs() {
    printf("IDIVS\n");
}

void gen_lt(char *var, char *symb1, char *symb2) {
    printf("LT %s %s %s\n", var, symb1, symb2);
}

void gen_gt(char *var, char *symb1, char *symb2) {
    printf("GT %s %s %s\n", var, symb1, symb2);
}

void gen_eq(char *var, char *symb1, char *symb2) {
    printf("EQ %s %s %s\n", var, symb1, symb2);
}

void gen_lts() {
    printf("LTS\n");
}

void gen_gts() {
    printf("GTS\n");
}

void gen_eqs() {
    printf("EQS\n");
}

void gen_and(char *var, char *symb1, char *symb2) {
    printf("AND %s %s %s\n", var, symb1, symb2);
}

void gen_or(char *var, char *symb1, char *symb2) {
    printf("OR %s %s %s\n", var, symb1, symb2);
}

void gen_not(char *var, char *symb) {
    printf("NOT %s %s %s\n", var, symb);
}

void gen_ands() {
    printf("ANDS\n");
}

void gen_ors() {
    printf("ORS\n");
}

void gen_nots() {
    printf("NOTS\n");
}

void gen_int2float(char *var, char *symb) {
    printf("INT2FLOAT %s %s %s\n", var, symb);
}

void gen_float2int(char *var, char *symb) {
    printf("FLOAT2INT %s %s %s\n", var, symb);
}

void gen_int2char(char *var, char *symb) {
    printf("INT2CHAR %s %s %s\n", var, symb);
}

void gen_stri2int(char *var, char *symb1, char *symb2) {
    printf("STRI2INT %s %s %s\n", var, symb1, symb2);
}

// ---------------------------------------------------| Vstupne-vystupni instrukce
void gen_int2floats() {
    printf("INT2FLOATS\n");
}

void gen_float2ints() {
    printf("FLOAT2INTS\n");
}

void gen_int2chars() {
    printf("INT2CHARS\n");
}

void gen_stri2ints() {
    printf("STRI2INTS\n");
}

void gen_read(char *var, char *type) {
    printf("READ %s %s\n", var, type);
}

void gen_write(char *symb) {
    printf("WRITE %s\n", symb);
}

void gen_concat(char *var, char *symb1, char *symb2) {
    printf("CONCAT %s %s %s\n", var, symb1, symb2);
}

void gen_strlen(char *var, char *symb) {
    printf("STRLEN %s %s\n", var, symb);
}

void gen_getchar(char *var, char *symb1, char *symb2) {
    printf("GETCHAR %s %s %s\n", var, symb1, symb2);
}

void gen_setchar(char *var, char *symb1, char *symb2) {
    printf("SETCHAR %s %s %s\n", var, symb1, symb2);
}

// ---------------------------------------------------| Prace s typy
void gen_type(char *var, char *type) {
    printf("TYPE %s %s\n", var, type);
}

// ---------------------------------------------------| Instrukce pro rizeni toku programu
void gen_label(char *label) {
    printf("LABEL %s\n", label);
}

void gen_jump(char *label) {
    printf("JUMP %s\n", label);
}

void gen_jumpifeq(char *label, char *symb1, char *symb2) {
    printf("JUMPIFEQ %s %s %s\n", label, symb1, symb2);
}

void gen_jumpifneq(char *label, char *symb1, char *symb2) {
    printf("JUMPIFNEQ %s %s %s\n", label, symb1, symb2);
}

void gen_jumpifeqs(char *label) {
    printf("JUMPIFEQS %s\n", label);
}

void gen_jumpifneqs(char *label) {
    printf("JUMPIFNEQS %s\n", label);
}

void gen_exit(char *symb) {
    printf("EXIT %s\n", symb);
}

// ---------------------------------------------------| Ladici instrukce
void gen_break() {
    printf("BREAK\n");
}

void gen_dprint(char *symb) {
    printf("DPRINT %s\n", symb);
}

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
#include "string_functions.h"
#include "symtable.h"
#include "token_types.h"

// IFJcode20 converted to C string by:
// https://tomeko.net/online_tools/cpp_text_escape.php?lang=en

void gen_enter_main() {
    printf(".IFJcode20\n");
    gen_jump("$$main");

    printf("\n# ----------------------------------- main()\n");
    gen_label("$$main");
    // Create local frame for the scope of main()
    gen_createframe();
    gen_pushframe();
    printf("\n");
}

void gen_exit_main() {
    gen_exit("int@0");
}

void gen_enter_function_scope() {
    gen_createframe();
    gen_pushframe();
}

void gen_leave_function_scope() {
    gen_popframe();
}

void gen_assign_token_to_var(char *var_name, tokenT *token) {
    stringT str_to_assign;
    stringT str_var_with_frame;

    string_init(&str_to_assign);
    string_init(&str_var_with_frame);

    char *assigned_data_type;

    switch (token->token_type) {
        case TOKEN_STRING_LITERAL:
            assigned_data_type = "string@";
            break;
        case TOKEN_INTEGER_LITERAL:
            assigned_data_type = "int@";
            break;
        case TOKEN_EXPONENT_LITERAL:
        case TOKEN_DECIMAL_LITERAL:
            assigned_data_type = "float@";
            break;
        case TOKEN_IDENTIFIER:

            break;
        default:
            break;
    }

    string_add_string(&str_to_assign, assigned_data_type);
    if (token->token_type == TOKEN_EXPONENT_LITERAL || token->token_type == TOKEN_DECIMAL_LITERAL) {
        char str_hex_float[1000];
        sprintf(str_hex_float, "%a", strtof(token->attribute.string_val.string, NULL));
        string_add_string(&str_to_assign, str_hex_float);
    } else {
        string_add_string(&str_to_assign, token->attribute.string_val.string);
    }

    string_add_string(&str_var_with_frame, "LF@");
    string_add_string(&str_var_with_frame, var_name);
    gen_move(str_var_with_frame.string, str_to_assign.string);
}

void gen_print(tokenT *token_to_print) {
    stringT var_or_symb;
    string_init(&var_or_symb);

    char *tok_attr_str = token_to_print->attribute.string_val.string;

    switch (token_to_print->token_type) {
        case TOKEN_IDENTIFIER:
            string_add_string(&var_or_symb, "LF@");
            string_add_string(&var_or_symb, tok_attr_str);
            break;
        case TOKEN_INTEGER_LITERAL:
            string_add_string(&var_or_symb, "int@");
            string_add_string(&var_or_symb, tok_attr_str);
            break;
        case TOKEN_STRING_LITERAL:
            string_add_string(&var_or_symb, "string@");

            for (int i = 0; tok_attr_str[i] != '\0'; i++) {
                if ((tok_attr_str[i] >= 0 && tok_attr_str[i] <= 32) || tok_attr_str[i] == 35 || tok_attr_str[i] == 92) {
                    if (tok_attr_str[i] < 10){
                        string_add_string(&var_or_symb, "\\00");
                        char str_digit[2];
                        sprintf(str_digit, "%d", tok_attr_str[i]);
                        string_add_string(&var_or_symb, str_digit);
                    } else {
                        string_add_string(&var_or_symb, "\\0");
                        char str_two_digits[3];
                        sprintf(str_two_digits, "%d", tok_attr_str[i]);
                        string_add_string(&var_or_symb, str_two_digits);
                    }
                } else {
                    string_add_character(&var_or_symb, tok_attr_str[i]);
                }
            }

            break;
        case TOKEN_DECIMAL_LITERAL:
        case TOKEN_EXPONENT_LITERAL:
            string_add_string(&var_or_symb, "float@");
            char float_str[1000];
            sprintf(float_str, "%a", strtof(tok_attr_str,NULL));
            string_add_string(&var_or_symb, float_str);
            break;
    }

    gen_write(var_or_symb.string);
}

void gen_call_input(int func_token_type, tokenT *token_array, int tok_index) {
    gen_createframe();

    if (func_token_type == TOKEN_FUNCTION_INPUTS){
        gen_call("$inputs"); // inputs()
    } else if (func_token_type == TOKEN_FUNCTION_INPUTF) {
        gen_call("$inputf"); // inputf()
    } else if (func_token_type == TOKEN_FUNCTION_INPUTI) {
        gen_call("$inputi"); // inputi()
    }

    tokenT first_var = token_array[tok_index - 4];
    tokenT second_var = token_array[tok_index - 2];

    if (first_var.token_type != TOKEN_UNDERSCORE) {
        gen_move_to_lf(first_var.attribute.string_val.string, RETURN_VALUE_1);
    }

    if (second_var.token_type != TOKEN_UNDERSCORE) {
        gen_move_to_lf(second_var.attribute.string_val.string, RETURN_VALUE_2);
    }
}

void gen_def_inputs() {
    printf(
            "\n# ----------------------------------- inputs()\n"
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
    printf("\n# ----------------------------------- inputi()\n"
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
    printf("\n# ----------------------------------- inputf()\n"
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

void gen_def_builtin_functions(const bool builtin_func_used[]) {
    printf("\n\n# Built-in function definitions (if any are used)\n\n");

    if (builtin_func_used[0] == true) // generate inputs
        gen_def_inputs();
    if (builtin_func_used[1] == true) // generate inputi
        gen_def_inputi();
    if (builtin_func_used[2] == true) // generate inputf
        gen_def_inputf();
    if (builtin_func_used[3] == true) // generate int2float
        ;
    if (builtin_func_used[4] == true) // generate float2int
        ;
    if (builtin_func_used[5] == true) // generate len
        ;
    if (builtin_func_used[6] == true) // generate substr
        ;
    if (builtin_func_used[7] == true) // generate ord
        ;
    if (builtin_func_used[8] == true) // generate chr
        ;
}

// ---------------------------------------------------| Prace s ramci, volani funkci
void gen_move(char *var, char *symb) {
    printf("MOVE %s %s\n", var, symb);
}

void gen_move_to_lf(char *var, char *symb) {
    printf("MOVE LF@%s %s\n", var, symb);
}

void gen_move_to_tf(char *var, char *symb) {
    printf("MOVE TF@%s %s\n", var, symb);
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

void gen_defvar_lf(char *var) {
    printf("DEFVAR LF@%s\n", var);
}

void gen_defvar_tf(char *var) {
    printf("DEFVAR TF@%s\n", var);
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
    printf("NOT %s %s\n", var, symb);
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

void gen_int2float(char *var, tokenT *symb_token) {
    if (symb_token->token_type == TOKEN_IDENTIFIER) {
        printf("INT2FLOAT LF@%s LF@%s\n", var, symb_token->attribute.string_val.string);

    } else {
        printf("INT2FLOAT LF@%s int@%s\n", var, symb_token->attribute.string_val.string);
    }
}

void gen_float2int(char *var, tokenT *symb_token) {
    if (symb_token->token_type == TOKEN_IDENTIFIER) {
        printf("FLOAT2INT LF@%s LF@%s\n", var, symb_token->attribute.string_val.string);

    } else {
        char str_hex_float[1000];
        sprintf(str_hex_float, "%a", strtof(symb_token->attribute.string_val.string, NULL));
        printf("FLOAT2INT LF@%s float@%s\n", var, str_hex_float);
    }
}

void gen_int2char(char *var, char *symb) {
    printf("INT2CHAR %s %s\n", var, symb);
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
    printf("\nEXIT %s\n", symb);
}

// ---------------------------------------------------| Ladici instrukce
void gen_break() {
    printf("BREAK\n");
}

void gen_dprint(char *symb) {
    printf("DPRINT %s\n", symb);
}

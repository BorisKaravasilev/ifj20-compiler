/**
 * Project: Implementation of a compiler of the IFJ20 language.
 *
 * @file generator.c
 * @brief Functions for generating "IFJcode20"
 * @author Boris Karavasilev <xkarav01@stud.fit.vutbr.cz>
 * @author Dominik Vecera <xvecer23@stud.fit.vutbr.cz>
 * @date 21. 11. 2020
 */

#include <stdio.h>

#include "generator.h"
#include "string_functions.h"
#include "symtable.h"
#include "token_types.h"
#include "utility_functions.h"

// IFJcode20 converted to C string by:
// https://tomeko.net/online_tools/cpp_text_escape.php?lang=en

void gen_enter_main() {
    printf(".IFJcode20\n");
    gen_jump("$main");
}

void gen_start_func(char *name) {
    printf("\n# ----------------------------------- %s()\n", name);
    char label[100];
    sprintf(label, "$%s", name);

    gen_label(label);
    // Push or create local frame for the scope of the function()
    if (strcmp(label, "$main") == 0){
        gen_createframe();
    }
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

void gen_escape_string(char *orig, stringT *escaped){
    for (int i = 0; orig[i] != '\0'; i++) {
        if ((orig[i] >= 0 && orig[i] <= 32) || orig[i] == 35 || orig[i] == 92) {
            if (orig[i] < 10){
                string_add_string(escaped, "\\00");
                char str_digit[2];
                sprintf(str_digit, "%d", orig[i]);
                string_add_string(escaped, str_digit);
            }
            else if (orig[i] == 92) {
                if (orig[i + 1] == 34) {    // "
                    string_add_character(escaped, '\"');
                    i++;
                }
                else if (orig[i + 1] == 110) {  // \n
                    string_add_string(escaped, "\\010");
                    i++;
                }
                else if (orig[i + 1] == 116){   // \t
                    string_add_string(escaped, "\\009");
                    i++;
                }
                else if (orig[i + 1] == 92){    // "\"
                    string_add_string(escaped, "\\092");
                    i++;
                }
                else if (orig[i + 1] == 120){    // x
                    if (check_hex(orig[i + 2]) && check_hex(orig[i + 3])){
                        char str_hex[5];
                        sprintf(str_hex, "0x%c%c", orig[i + 2], orig[i + 3]);
                        long n = strtol(str_hex, NULL, 16);
                        string_add_character(escaped, (char) n);
                        i += 3;
                    }
                    else {
                        fprintf(stderr, "Invalid escape sequence.\n");
                        exit(1);
                    }
                }
                else {
                    fprintf(stderr, "Invalid escape sequence.\n");
                    exit(1);
                }
            }
            else {
                string_add_string(escaped, "\\0");
                char str_two_digits[3];
                sprintf(str_two_digits, "%d", orig[i]);
                string_add_string(escaped, str_two_digits);
            }
        } else {
            string_add_character(escaped, orig[i]);
        }
    }
}

void gen_assign_token_to_var(char *var_name, tokenT *token) {
    stringT str_to_assign;
    stringT str_var_with_frame;

    string_init(&str_to_assign);
    string_init(&str_var_with_frame);

    string_add_string(&str_var_with_frame, "LF@");
    string_add_string(&str_var_with_frame, var_name);

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
            assigned_data_type = "LF@";
            break;

        default:
            assigned_data_type = "nil@";
            break;
    }

    string_add_string(&str_to_assign, assigned_data_type);
    if (token->token_type == TOKEN_EXPONENT_LITERAL || token->token_type == TOKEN_DECIMAL_LITERAL) {
        char str_hex_float[1000];
        sprintf(str_hex_float, "%a", strtof(token->attribute.string_val.string, NULL));
        string_add_string(&str_to_assign, str_hex_float);
    }
    else if (token->token_type == TOKEN_STRING_LITERAL){
        gen_escape_string(token->attribute.string_val.string, &str_to_assign);
    }
    else if (token->token_type == TOKEN_IDENTIFIER || token->token_type == TOKEN_INTEGER_LITERAL) {
        string_add_string(&str_to_assign, token->attribute.string_val.string);
    }
    else {
        string_add_string(&str_to_assign, "nil");
    }

    gen_move(str_var_with_frame.string, str_to_assign.string);
}

void gen_print_type(tokenT *param_token){
    switch (param_token->token_type) {
        case TOKEN_STRING_LITERAL:
            printf("string@");
            break;

        case TOKEN_INTEGER_LITERAL:
            printf("int@");
            break;

        case TOKEN_EXPONENT_LITERAL:
        case TOKEN_DECIMAL_LITERAL:
            printf("float@");
            break;

        case TOKEN_IDENTIFIER:
            printf("LF@");
            break;

        default:
            printf("nil@");
            break;
    }
}

void gen_parameter(tokenT *param_token, int param_number){
    stringT right_side_str;
    string_init(&right_side_str);

    char *assigned_data_type;

    switch (param_token->token_type) {
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
            assigned_data_type = "LF@";
            break;

        default:
            assigned_data_type = "nil@";
            break;
    }

    string_add_string(&right_side_str, assigned_data_type);
    if (param_token->token_type == TOKEN_EXPONENT_LITERAL || param_token->token_type == TOKEN_DECIMAL_LITERAL) {
        char str_hex_float[1000];
        sprintf(str_hex_float, "%a", strtof(param_token->attribute.string_val.string, NULL));
        string_add_string(&right_side_str, str_hex_float);
    }
    else if (param_token->token_type == TOKEN_STRING_LITERAL){
        gen_escape_string(param_token->attribute.string_val.string, &right_side_str);
    }
    else if (param_token->token_type == TOKEN_IDENTIFIER || param_token->token_type == TOKEN_INTEGER_LITERAL) {
        string_add_string(&right_side_str, param_token->attribute.string_val.string);
    }
    else {
        string_add_string(&right_side_str, "nil");
    }

    printf("MOVE TF@%%%d %s\n", param_number, right_side_str.string);
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
            gen_escape_string(tok_attr_str, &var_or_symb);
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

    // Call input function
    if (func_token_type == TOKEN_FUNCTION_INPUTS){
        gen_call("$inputs"); // inputs()
    } else if (func_token_type == TOKEN_FUNCTION_INPUTF) {
        gen_call("$inputf"); // inputf()
    } else if (func_token_type == TOKEN_FUNCTION_INPUTI) {
        gen_call("$inputi"); // inputi()
    }

    // Store function results
    tokenT first_var = token_array[tok_index - 6];
    tokenT second_var = token_array[tok_index - 4];

    if (first_var.token_type != TOKEN_UNDERSCORE) {
        gen_move_to_lf(first_var.attribute.string_val.string, RETURN_VALUE_1);
    }

    if (second_var.token_type != TOKEN_UNDERSCORE) {
        gen_move_to_lf(second_var.attribute.string_val.string, RETURN_VALUE_2);
    }
}

void gen_call_substr(tokenT *token_array, int tok_index) {
    gen_createframe();

    tokenT first_var = token_array[tok_index - 11];
    tokenT second_var = token_array[tok_index - 9];
    tokenT first_param = token_array[tok_index - 5];
    tokenT second_param = token_array[tok_index - 3];
    tokenT third_param = token_array[tok_index - 1];

    // Define and init variables for parameters
    stringT param1;
    string_init(&param1);
    stringT param2;
    string_init(&param2);
    stringT param3;
    string_init(&param3);

    // Get first parameter content
    if (first_param.token_type == TOKEN_IDENTIFIER){
        string_add_string(&param1, "LF@");
        string_add_string(&param1, first_param.attribute.string_val.string);
    }
    else {  // string literal
        string_add_string(&param1, "string@");
        gen_escape_string(first_param.attribute.string_val.string, &param1);
    }

    // Get second parameter content
    if (second_param.token_type == TOKEN_IDENTIFIER){
        string_add_string(&param2, "LF@");
    }
    else {  // integer literal
        string_add_string(&param2, "int@");
    }
    string_add_string(&param2, second_param.attribute.string_val.string);

    // Get third parameter content
    if (third_param.token_type == TOKEN_IDENTIFIER){
        string_add_string(&param3, "LF@");
    }
    else {  // integer literal
        string_add_string(&param3, "int@");
    }
    string_add_string(&param3, third_param.attribute.string_val.string);

    // Create temporary variables for parameters and call function
    gen_defvar_tf("%1");
    gen_move_to_tf("%1", param1.string);
    gen_defvar_tf("%2");
    gen_move_to_tf("%2", param2.string);
    gen_defvar_tf("%3");
    gen_move_to_tf("%3", param3.string);

    gen_call("$substr");

    // Store function results
    if (first_var.token_type != TOKEN_UNDERSCORE) {
        gen_move_to_lf(first_var.attribute.string_val.string, RETURN_VALUE_1);
    }

    if (second_var.token_type != TOKEN_UNDERSCORE) {
        gen_move_to_lf(second_var.attribute.string_val.string, RETURN_VALUE_2);
    }
}

void gen_call_ord(tokenT *token_array, int tok_index) {
    gen_createframe();

    tokenT first_var = token_array[tok_index - 9];
    tokenT second_var = token_array[tok_index - 7];
    tokenT first_param = token_array[tok_index - 3];
    tokenT second_param = token_array[tok_index - 1];

    // Define and init variables for parameters
    stringT param1;
    string_init(&param1);
    stringT param2;
    string_init(&param2);

    // Get first parameter content
    if (first_param.token_type == TOKEN_IDENTIFIER){
        string_add_string(&param1, "LF@");
        string_add_string(&param1, first_param.attribute.string_val.string);
    }
    else {  // string literal
        string_add_string(&param1, "string@");
        gen_escape_string(first_param.attribute.string_val.string, &param1);
    }

    // Get second parameter content
    if (second_param.token_type == TOKEN_IDENTIFIER){
        string_add_string(&param2, "LF@");
    }
    else {  // integer literal
        string_add_string(&param2, "int@");
    }
    string_add_string(&param2, second_param.attribute.string_val.string);

    // Create temporary variables for parameters and call function
    gen_defvar_tf("%1");
    gen_move_to_tf("%1", param1.string);
    gen_defvar_tf("%2");
    gen_move_to_tf("%2", param2.string);

    gen_call("$ord");

    // Store function results
    if (first_var.token_type != TOKEN_UNDERSCORE) {
        gen_move_to_lf(first_var.attribute.string_val.string, RETURN_VALUE_1);
    }

    if (second_var.token_type != TOKEN_UNDERSCORE) {
        gen_move_to_lf(second_var.attribute.string_val.string, RETURN_VALUE_2);
    }
}


void gen_call_chr(tokenT *token_array, int tok_index) {
    gen_createframe();

    tokenT first_var = token_array[tok_index - 7];
    tokenT second_var = token_array[tok_index - 5];
    tokenT first_param = token_array[tok_index - 1];

    // Define and init variable for parameter
    stringT param1;
    string_init(&param1);

    // Get first parameter content
    if (first_param.token_type == TOKEN_IDENTIFIER){
        string_add_string(&param1, "LF@");
    }
    else {  // integer literal
        string_add_string(&param1, "int@");
    }
    string_add_string(&param1, first_param.attribute.string_val.string);

    // Create temporary variables for parameters and call function
    gen_defvar_tf("%1");
    gen_move_to_tf("%1", param1.string);

    gen_call("$chr");

    // Store function results
    if (first_var.token_type != TOKEN_UNDERSCORE) {
        gen_move_to_lf(first_var.attribute.string_val.string, RETURN_VALUE_1);
    }

    if (second_var.token_type != TOKEN_UNDERSCORE) {
        gen_move_to_lf(second_var.attribute.string_val.string, RETURN_VALUE_2);
    }
}

void gen_def_inputs() {
    printf("\n# ----------------------------------- inputs()\n"
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

void gen_def_substr(){
    printf("\n# ----------------------------------- substr()\n"
           "LABEL $substr\n"
           "PUSHFRAME\n"
           "\n"
           "# init return values\n"
           "DEFVAR LF@retval1\n"
           "MOVE LF@retval1 string@\n"
           "DEFVAR LF@retval2\n"
           "MOVE LF@retval2 int@1\n"
           "\n"
           "# init parameters\n"
           "DEFVAR LF@param1\n"
           "MOVE LF@param1 LF@%%1\n"
           "DEFVAR LF@param2\n"
           "MOVE LF@param2 LF@%%2\n"
           "DEFVAR LF@param3\n"
           "MOVE LF@param3 LF@%%3\n"
           "\n"
           "# function body\n"

           "# define temporary variables\n"
           "DEFVAR LF@cond                             # for comparison results\n"
           "DEFVAR LF@currchar                         # for storing the current character\n"
           "MOVE LF@currchar string@\n"
           "DEFVAR LF@index                            # for storing the current index\n"
           "DEFVAR LF@rch                              # number of remaining chars\n"
           "\n"
           "# check if start position of substring < 0\n"
           "LT LF@cond LF@param2 int@0                 # param2 < 0 ?\n"
           "JUMPIFEQ $substr$end LF@cond bool@true     # if yes, jump to $substr$end\n"
           "\n"
           "# get param1 string length and subtract 1 since indices start at 0\n"
           "DEFVAR LF@len\n"
           "STRLEN LF@len LF@param1\n"
           "SUB LF@len LF@len int@1\n"
           "\n"
           "# check if start position of substring > strlen(param1) - 1\n"
           "GT LF@cond LF@param2 LF@len                # param2 > strlen(param1) - 1 ?\n"
           "JUMPIFEQ $substr$end LF@cond bool@true     # if yes, jump to $substr$end\n"
           "\n"
           "# check if length of substring < 0\n"
           "LT LF@cond LF@param3 int@0                 # param3 < 0 ?\n"
           "JUMPIFEQ $substr$end LF@cond bool@true     # if yes, jump to $substr$end\n"
           "\n"
           "# parameters OK, get value of len(s) - i\n"
           "ADD LF@len LF@len int@1\n"
           "SUB LF@rch LF@len LF@param2\n"
           "\n"
           "# check if n > len(s) - i\n"
           "MOVE LF@index LF@param2\n"
           "GT LF@cond LF@param3 LF@rch                # n > len(s) - i ?\n"
           "JUMPIFEQ $substr$loop LF@cond bool@true    # if yes, jump to $substr$else\n"
           "\n"
           "# return only part of remaining substring (loop n times from i)\n"
           "MOVE LF@rch LF@param3\n"
           "\n"
           "# else return entire remaining substring (loop from i until len(s) -> len(s) - i times)\n"
           "\n"
           "# loop\n"
           "LABEL $substr$loop\n"
           "\n"
           "# check for loop ending\n"
           "EQ LF@cond LF@rch int@0                        # have all characters been read?\n"
           "JUMPIFEQ $substr$loopend LF@cond bool@true     # if yes, jump to $substr$loopend\n"
           "\n"
           "# get the current character and append it to the resulting string\n"
           "GETCHAR LF@currchar LF@param1 LF@index\n"
           "CONCAT LF@retval1 LF@retval1 LF@currchar\n"
           "SUB LF@rch LF@rch int@1\n"
           "ADD LF@index LF@index int@1\n"
           "JUMP $substr$loop\n"
           "\n"
           "# end of loop\n"
           "LABEL $substr$loopend\n"
           "MOVE LF@retval2 int@0\n"
           "\n"
           "# end\n"
           "LABEL $substr$end"

           "\n"
           "POPFRAME\n"
           "RETURN\n");
}

void gen_def_ord(){
    printf("\n# ----------------------------------- ord()\n"
           "LABEL $ord\n"
           "PUSHFRAME\n"
           "\n"
           "# init return values\n"
           "DEFVAR LF@retval1\n"
           "MOVE LF@retval1 int@0\n"
           "DEFVAR LF@retval2\n"
           "MOVE LF@retval2 int@1\n"
           "\n"
           "# init parameters\n"
           "DEFVAR LF@param1\n"
           "MOVE LF@param1 LF@%%1\n"
           "DEFVAR LF@param2\n"
           "MOVE LF@param2 LF@%%2\n"
           "\n"
           "# function body\n"

           "# define temporary variable for comparison results\n"
           "DEFVAR LF@cond\n"
           "\n"
           "# check if position in string < 0\n"
           "LT LF@cond LF@param2 int@0                 # param2 < 0 ?\n"
           "JUMPIFEQ $ord$end LF@cond bool@true      # if yes, jump to $ord$end\n"
           "\n"
           "# get param1 string length and subtract 1 since indices start at 0\n"
           "DEFVAR LF@len\n"
           "STRLEN LF@len LF@param1\n"
           "SUB LF@len LF@len int@1\n"
           "\n"
           "# check if position in string > strlen(param1) - 1\n"
           "GT LF@cond LF@param2 LF@len                # param2 > strlen(param1) - 1 ?\n"
           "JUMPIFEQ $ord$end LF@cond bool@true      # if yes, jump to $ord$end\n"
           "\n"
           "# if position is correct, get ordinal value of character at that position\n"
           "STRI2INT LF@retval1 LF@param1 LF@param2\n"
           "MOVE LF@retval2 int@0\n"
           "\n"
           "# end\n"
           "LABEL $ord$end\n"

           "\n"
           "POPFRAME\n"
           "RETURN\n");
}

void gen_def_chr(){
    printf("\n# ----------------------------------- chr()\n"
           "LABEL $chr\n"
           "PUSHFRAME\n"
           "\n"
           "# init return values\n"
           "DEFVAR LF@retval1\n"
           "MOVE LF@retval1 string@\n"
           "DEFVAR LF@retval2\n"
           "MOVE LF@retval2 int@1\n"
           "\n"
           "# init parameters\n"
           "DEFVAR LF@param1\n"
           "MOVE LF@param1 LF@%%1\n"
           "\n"
           "# function body\n"

           "# define temporary variable for comparison results\n"
           "DEFVAR LF@cond\n"
           "\n"
           "# check if ASCII code < 0\n"
           "LT LF@cond LF@param1 int@0               # param1 < 0 ?\n"
           "JUMPIFEQ $chr$end LF@cond bool@true      # if yes, jump to $chr$end\n"
           "\n"
           "# check if ASCII code > 255\n"
           "GT LF@cond LF@param1 int@255             # param1 > 255 ?\n"
           "JUMPIFEQ $chr$end LF@cond bool@true      # if yes, jump to $chr$end\n"
           "\n"
           "# if index is in ASCII range, get character of that index\n"
           "INT2CHAR LF@retval1 LF@param1\n"
           "MOVE LF@retval2 int@0\n"
           "\n"
           "# end\n"
           "LABEL $chr$end"

           "\n"
           "POPFRAME\n"
           "RETURN\n");
}

void gen_def_builtin_functions(const bool builtin_func_used[]) {
    printf("\n\n# Built-in function definitions (if any are used)\n");

    if (builtin_func_used[0] == true) // generate inputs
        gen_def_inputs();
    if (builtin_func_used[1] == true) // generate inputi
        gen_def_inputi();
    if (builtin_func_used[2] == true) // generate inputf
        gen_def_inputf();
    if (builtin_func_used[3] == true) // generate substr
        gen_def_substr();
    if (builtin_func_used[4] == true) // generate ord
        gen_def_ord();
    if (builtin_func_used[5] == true) // generate chr
        gen_def_chr();
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
    printf("\nPOPFRAME\n");
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

void gen_strlen(char *var, tokenT *symb_token) {
    if (symb_token->token_type == TOKEN_IDENTIFIER) {
        printf("STRLEN LF@%s LF@%s\n", var, symb_token->attribute.string_val.string);
    } else {
        printf("STRLEN LF@%s string@%s\n", var, symb_token->attribute.string_val.string);

    }
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

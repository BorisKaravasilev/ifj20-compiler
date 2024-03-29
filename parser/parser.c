/**
 * Project: Implementation of a compiler of the IFJ20 language.
 *
 * @file parser.c
 * @brief Parser implementation
 * @author Dominik Vecera <xvecer23@stud.fit.vutbr.cz>
 * @author Petr Vrtal <xvrtal01@stud.fit.vutbr.cz>
 */

#include "parser.h"
#include "debugging.h"
#include "generator.h"

/// Necessary information for correct processing and generating
// Number of assignments on the right side
int assigns_right = 0;
// Current level of indentation
int indent_level = 0;
// The index of the current token
int token_index = 0;
// Variable for storing the result of recursive function calls
int tmp_result = 0;

// For different handling of "right side" values in return than in regular assignments
bool in_return = false;
// To prevent redefinition of variables in for cycles
bool in_for = false;
// Expression analysis needs to check one extra token, so this sets the unget_token flag
bool was_expr = false;
// For different handling of single and multiple assignments
bool single_assign = false;
// Skip token reading if an extra token was read
bool unget_token = false;

// String for saving definitions inside cycles to avoid redefinitions
stringT defvars_in_for;

/* Store information whether to generate individual built-in function definitions,
   order is the same as in token_types.h, but some are excluded */
bool builtin_func_used[6] = { false };

// Structures for semantic checks
assignmentT assignment_check_struct;
late_check_stack semantic_late_check_stack;

// Tokens for generating multiple assignments
tokenT left_side_assignments[LEFT_SIDE_TOKEN_COUNT];

void err_print(char* str, int token_type){
    fprintf(stderr, "Expected %s but got token %d (as defined in token_types.h).\n", str, token_type);
}

int id(scannerT *ptr_scanner, tokenT token[], bool assign_allowed){
    if (!unget_token) {
        get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // identifier or _
    }
    else {
        unget_token = false;
    }

    if (token[token_index].token_type == TOKEN_UNDERSCORE) {
        // SEMANTIC
        if (assign_allowed) {
            assignment_add_identifier(&assignment_check_struct, token[token_index].token_type, NULL);
        }
        // END SEMANTIC
        return SYNTAX_OK;
    }
    else if (token[token_index].token_type == TOKEN_IDENTIFIER) {
        // SEMANTIC
        if (assign_allowed) {
            ST_Item *symbol;
            if ((symbol = stack_search(&ptr_scanner->st_stack, &token[token_index].attribute.string_val)) == NULL) {
                fprintf(stderr, "Error: Undefined variable \'%s\'\n", token[token_index].attribute.string_val.string);
                return RC_SEMANTIC_IDENTIFIER_ERR;
            }
            assignment_add_identifier(&assignment_check_struct, token[token_index].token_type, symbol);
        }
        // END SEMANTIC
        return SYNTAX_OK;
    }

    return RC_SYN_ERR;
}

int literal(scannerT *ptr_scanner, tokenT token[], int *item_type){
    if (!unget_token) {
        get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // int, float or string literal
    }
    else {
        unget_token = false;
    }

    if (token[token_index].token_type == TOKEN_INTEGER_LITERAL ||
        token[token_index].token_type == TOKEN_DECIMAL_LITERAL ||
        token[token_index].token_type == TOKEN_EXPONENT_LITERAL ||
        token[token_index].token_type == TOKEN_STRING_LITERAL) {

        if (item_type != NULL) {
            switch(token[token_index].token_type) {
                case TOKEN_INTEGER_LITERAL:
                    *item_type = TYPE_INT;
                    break;
                case TOKEN_DECIMAL_LITERAL:
                case TOKEN_EXPONENT_LITERAL:
                    *item_type = TYPE_DECIMAL;
                    break;
                case TOKEN_STRING_LITERAL:
                    *item_type = TYPE_STRING;
                    break;
                default:
                    break;
            }
        }
        return SYNTAX_OK;
    }

    return RC_SYN_ERR;
}

int item(scannerT *ptr_scanner, tokenT token[], int *item_type){
    if (!unget_token) {
        get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // int, float or string keyword
    }
    else {
        unget_token = false;
    }

    if (token[token_index].token_type == TOKEN_KEYWORD_INT ||
        token[token_index].token_type == TOKEN_KEYWORD_FLOAT64 ||
        token[token_index].token_type == TOKEN_KEYWORD_STRING) {

        if (item_type != NULL) {
            switch(token[token_index].token_type) {
                case TOKEN_KEYWORD_INT:
                    *item_type = TYPE_INT;
                    break;
                case TOKEN_KEYWORD_FLOAT64:
                    *item_type = TYPE_DECIMAL;
                    break;
                case TOKEN_KEYWORD_STRING:
                    *item_type = TYPE_STRING;
                    break;
                default:
                    break;
            }
        }
        return SYNTAX_OK;
    }

    return RC_SYN_ERR;
}

int print_next(scannerT *ptr_scanner, tokenT token[]){
    if (!unget_token) {
        get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // , or )
    }
    else {
        unget_token = false;
    }

    switch (token[token_index].token_type) {
        case TOKEN_RIGHT_BRACKET:
            return SYNTAX_OK;

        case TOKEN_COMMA:
            if (!unget_token) {
                get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // int / float / str literal, id
            }
            else {
                unget_token = false;
            }

            if (token[token_index].token_type == TOKEN_IDENTIFIER){
                // GENERATE
                gen_print(&token[token_index]);

                return print_next(ptr_scanner, token);
            }
            else {
                unget_token = true;
                tmp_result = literal(ptr_scanner, token, NULL);
                if (tmp_result != SYNTAX_OK)
                    return tmp_result;

                // GENERATE
                gen_print(&token[token_index]);

                return print_next(ptr_scanner, token);
            }

        default:
            err_print(", or )", token[token_index].token_type);
            return RC_SYN_ERR;
    }
}

int print(scannerT *ptr_scanner, tokenT token[]){
    if (!unget_token) {
        get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // int / float / str literal, id, )
    }
    else {
        unget_token = false;
    }

    switch (token[token_index].token_type) {
        case TOKEN_RIGHT_BRACKET:
            return SYNTAX_OK;

        case TOKEN_IDENTIFIER:
            // GENERATE
            gen_print(&token[token_index]);

            return print_next(ptr_scanner, token);

        default:
            unget_token = true;
            tmp_result = literal(ptr_scanner, token, NULL);
            if (tmp_result != SYNTAX_OK)
                return tmp_result;

            // GENERATE
            gen_print(&token[token_index]);

            return print_next(ptr_scanner, token);
    }
}

int builtin_func(scannerT *ptr_scanner, tokenT token[], int *built_in_func_type){
    if (!unget_token) {
        get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // built-in function name
    }
    else {
        unget_token = false;
    }

    switch (token[token_index].token_type) {
        case TOKEN_FUNCTION_INPUTS:
        case TOKEN_FUNCTION_INPUTI:
        case TOKEN_FUNCTION_INPUTF:
            // store information about a specific used function to generate its code
            builtin_func_used[token[token_index].token_type - TOKEN_FUNCTION_INPUTS] = true;

            if (built_in_func_type != NULL) {
                *built_in_func_type = token[token_index].token_type;
            }
            if (!unget_token) {
                get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // (
            }
            else {
                unget_token = false;
            }

            if (token[token_index].token_type != TOKEN_LEFT_BRACKET){
                err_print("(", token[token_index].token_type);
                return RC_SYN_ERR;
            }

            if (!unget_token) {
                get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // )
            }
            else {
                unget_token = false;
            }

            if (token[token_index].token_type == TOKEN_RIGHT_BRACKET){
                // GENERATE
                if (token[token_index - 3].token_type == TOKEN_EQUAL) {
                    gen_call_input(token[token_index - 2].token_type, token, token_index);
                }

                return SYNTAX_OK;
            }
            else if (token[token_index].token_type == TOKEN_IDENTIFIER ||
                    token[token_index].token_type == TOKEN_INTEGER_LITERAL ||
                    token[token_index].token_type == TOKEN_STRING_LITERAL ||
                    token[token_index].token_type == TOKEN_DECIMAL_LITERAL ||
                    token[token_index].token_type == TOKEN_EXPONENT_LITERAL) {
                fprintf(stderr, "Error: Invalid parameter count supplied to input function\n");
                return RC_SEMANTIC_FUNC_PARAM_ERR;
            }
            else {
                err_print(")", token[token_index].token_type);
                return RC_SYN_ERR;
            }

        case TOKEN_FUNCTION_PRINT:
            if (built_in_func_type != NULL) {
                *built_in_func_type = token[token_index].token_type;
            }
            if (!unget_token) {
                get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // (
            }
            else {
                unget_token = false;
            }

            if (token[token_index].token_type != TOKEN_LEFT_BRACKET){
                err_print("(", token[token_index].token_type);
                return RC_SYN_ERR;
            }

            return print(ptr_scanner, token);

        case TOKEN_FUNCTION_INT2FLOAT:
            if (built_in_func_type != NULL) {
                *built_in_func_type = token[token_index].token_type;
            }
            if (!unget_token) {
                get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // (
            }
            else {
                unget_token = false;
            }

            if (token[token_index].token_type != TOKEN_LEFT_BRACKET){
                err_print("(", token[token_index].token_type);
                return RC_SYN_ERR;
            }

            if (!unget_token) {
                get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // int or id
            }
            else {
                unget_token = false;
            }

            if (token[token_index].token_type == TOKEN_IDENTIFIER){
                ST_Item *symbol;
                if ((symbol = stack_search(&ptr_scanner->st_stack, &token[token_index].attribute.string_val)) == NULL) {
                    fprintf(stderr, "Error: Undefined variable \'%s\'\n", token[token_index].attribute.string_val.string);
                    return RC_SEMANTIC_IDENTIFIER_ERR;
                } else if (symbol->type != TYPE_INT) {
                    fprintf(stderr, "Error: Invalid parameter type supplied to function \'%s\'\n", "int2float()");
                    return RC_SEMANTIC_FUNC_PARAM_ERR;
                }
            }
            else if (token[token_index].token_type != TOKEN_INTEGER_LITERAL){
                err_print("int or id", token[token_index].token_type);
                return RC_SEMANTIC_FUNC_PARAM_ERR;
            }

            if (!unget_token) {
                get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // )
            }
            else {
                unget_token = false;
            }

            if (token[token_index].token_type == TOKEN_RIGHT_BRACKET){
                // GENERATE int2float()
                if (token[token_index - 4].token_type == TOKEN_EQUAL){
                    char *var_to_assign_to = token[token_index - 5].attribute.string_val.string;
                    gen_int2float(var_to_assign_to, &token[token_index - 1]);
                }

                return SYNTAX_OK;
            }
            else if (token[token_index].token_type == TOKEN_COMMA){
                fprintf(stderr, "Error: Invalid parameter count supplied to function \'%s\'\n", "int2float()");
                return RC_SEMANTIC_FUNC_PARAM_ERR;
            }
            else {
                err_print(")", token[token_index].token_type);
                return RC_SYN_ERR;
            }

        case TOKEN_FUNCTION_FLOAT2INT:
            if (built_in_func_type != NULL) {
                *built_in_func_type = token[token_index].token_type;
            }
            if (!unget_token) {
                get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // (
            }
            else {
                unget_token = false;
            }

            if (token[token_index].token_type != TOKEN_LEFT_BRACKET){
                err_print("(", token[token_index].token_type);
                return RC_SYN_ERR;
            }

            if (!unget_token) {
                get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // float or id
            }
            else {
                unget_token = false;
            }

            if (token[token_index].token_type == TOKEN_IDENTIFIER){
                ST_Item *symbol;
                if ((symbol = stack_search(&ptr_scanner->st_stack, &token[token_index].attribute.string_val)) == NULL) {
                    fprintf(stderr, "Error: Undefined variable \'%s\'\n", token[token_index].attribute.string_val.string);
                    return RC_SEMANTIC_IDENTIFIER_ERR;
                } else if (symbol->type != TYPE_DECIMAL) {
                    fprintf(stderr, "Error: Invalid parameter type supplied to function \'%s\'\n", "float2int()");
                    return RC_SEMANTIC_FUNC_PARAM_ERR;
                }
            }
            else if (token[token_index].token_type != TOKEN_DECIMAL_LITERAL &&
                     token[token_index].token_type != TOKEN_EXPONENT_LITERAL){
                err_print("float or id", token[token_index].token_type);
                return RC_SEMANTIC_FUNC_PARAM_ERR;
            }

            if (!unget_token) {
                get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // )
            }
            else {
                unget_token = false;
            }

            if (token[token_index].token_type == TOKEN_RIGHT_BRACKET){
                // GENERATE float2int()
                if (token[token_index - 4].token_type == TOKEN_EQUAL){
                    char *var_to_assign_to = token[token_index - 5].attribute.string_val.string;
                    gen_float2int(var_to_assign_to, &token[token_index - 1]);
                }

                return SYNTAX_OK;
            }
            else if (token[token_index].token_type == TOKEN_COMMA){
                fprintf(stderr, "Error: Invalid parameter count supplied to function \'%s\'\n", "float2int()");
                return RC_SEMANTIC_FUNC_PARAM_ERR;
            }
            else {
                err_print(")", token[token_index].token_type);
                return RC_SYN_ERR;
            }

        case TOKEN_FUNCTION_LEN:
            if (built_in_func_type != NULL) {
                *built_in_func_type = token[token_index].token_type;
            }
            if (!unget_token) {
                get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // (
            }
            else {
                unget_token = false;
            }

            if (token[token_index].token_type != TOKEN_LEFT_BRACKET){
                err_print("(", token[token_index].token_type);
                return RC_SYN_ERR;
            }

            if (!unget_token) {
                get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // string or id
            }
            else {
                unget_token = false;
            }

            if (token[token_index].token_type == TOKEN_IDENTIFIER){
                ST_Item *symbol;
                if ((symbol = stack_search(&ptr_scanner->st_stack, &token[token_index].attribute.string_val)) == NULL) {
                    fprintf(stderr, "Error: Undefined variable \'%s\'\n", token[token_index].attribute.string_val.string);
                    return RC_SEMANTIC_IDENTIFIER_ERR;
                } else if (symbol->type != TYPE_STRING) {
                    fprintf(stderr, "Error: Invalid parameter type supplied to function \'%s\'\n", "len()");
                    return RC_SEMANTIC_FUNC_PARAM_ERR;
                }
            }
            else if (token[token_index].token_type != TOKEN_STRING_LITERAL){
                err_print("string or id", token[token_index].token_type);
                return RC_SEMANTIC_FUNC_PARAM_ERR;
            }

            if (!unget_token) {
                get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // )
            }
            else {
                unget_token = false;
            }

            if (token[token_index].token_type == TOKEN_RIGHT_BRACKET){
                // GENERATE len(s string) (int)
                if (token[token_index - 4].token_type == TOKEN_EQUAL){
                    char *var_to_assign_to = token[token_index - 5].attribute.string_val.string;
                    gen_strlen(var_to_assign_to, &token[token_index - 1]);
                }

                return SYNTAX_OK;
            }
            else if (token[token_index].token_type == TOKEN_COMMA){
                fprintf(stderr, "Error: Invalid parameter count supplied to function \'%s\'\n", "len()");
                return RC_SEMANTIC_FUNC_PARAM_ERR;
            }
            else {
                err_print(")", token[token_index].token_type);
                return RC_SYN_ERR;
            }

        case TOKEN_FUNCTION_SUBSTR:
            builtin_func_used[3] = true;
            if (built_in_func_type != NULL) {
                *built_in_func_type = token[token_index].token_type;
            }
            if (!unget_token) {
                get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // (
            }
            else {
                unget_token = false;
            }

            if (token[token_index].token_type != TOKEN_LEFT_BRACKET){
                err_print("(", token[token_index].token_type);
                return RC_SYN_ERR;
            }

            if (!unget_token) {
                get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // string or id
            }
            else {
                unget_token = false;
            }

            if (token[token_index].token_type == TOKEN_IDENTIFIER){
                ST_Item *symbol;
                if ((symbol = stack_search(&ptr_scanner->st_stack, &token[token_index].attribute.string_val)) == NULL) {
                    fprintf(stderr, "Error: Undefined variable \'%s\'\n", token[token_index].attribute.string_val.string);
                    return RC_SEMANTIC_IDENTIFIER_ERR;
                } else if (symbol->type != TYPE_STRING) {
                    fprintf(stderr, "Error: Invalid parameter type supplied to function \'%s\'\n", "substr()");
                    return RC_SEMANTIC_FUNC_PARAM_ERR;
                }
            }
            else if (token[token_index].token_type != TOKEN_STRING_LITERAL){
                err_print("string or id", token[token_index].token_type);
                return RC_SEMANTIC_FUNC_PARAM_ERR;
            }

            if (!unget_token) {
                get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // ,
            }
            else {
                unget_token = false;
            }

            if (token[token_index].token_type != TOKEN_COMMA){
                err_print(",", token[token_index].token_type);
                return RC_SYN_ERR;
            }

            if (!unget_token) {
                get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // int or id
            }
            else {
                unget_token = false;
            }

            if (token[token_index].token_type == TOKEN_IDENTIFIER){
                ST_Item *symbol;
                if ((symbol = stack_search(&ptr_scanner->st_stack, &token[token_index].attribute.string_val)) == NULL) {
                    fprintf(stderr, "Error: Undefined variable \'%s\'\n", token[token_index].attribute.string_val.string);
                    return RC_SEMANTIC_IDENTIFIER_ERR;
                } else if (symbol->type != TYPE_INT) {
                    fprintf(stderr, "Error: Invalid parameter type supplied to function \'%s\'\n", "substr()");
                    return RC_SEMANTIC_FUNC_PARAM_ERR;
                }
            }
            else if (token[token_index].token_type != TOKEN_INTEGER_LITERAL){
                err_print("int or id", token[token_index].token_type);
                return RC_SEMANTIC_FUNC_PARAM_ERR;
            }

            if (!unget_token) {
                get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // ,
            }
            else {
                unget_token = false;
            }

            if (token[token_index].token_type != TOKEN_COMMA){
                err_print(",", token[token_index].token_type);
                return RC_SYN_ERR;
            }

            if (!unget_token) {
                get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // int or id
            }
            else {
                unget_token = false;
            }

            if (token[token_index].token_type == TOKEN_IDENTIFIER){
                ST_Item *symbol;
                if ((symbol = stack_search(&ptr_scanner->st_stack, &token[token_index].attribute.string_val)) == NULL) {
                    fprintf(stderr, "Error: Undefined variable \'%s\'\n", token[token_index].attribute.string_val.string);
                    return RC_SEMANTIC_IDENTIFIER_ERR;
                } else if (symbol->type != TYPE_INT) {
                    fprintf(stderr, "Error: Invalid parameter type supplied to function \'%s\'\n", "substr()");
                    return RC_SEMANTIC_FUNC_PARAM_ERR;
                }
            }
            else if (token[token_index].token_type != TOKEN_INTEGER_LITERAL){
                err_print("int or id", token[token_index].token_type);
                return RC_SEMANTIC_FUNC_PARAM_ERR;
            }

            if (!unget_token) {
                get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // )
            }
            else {
                unget_token = false;
            }

            if (token[token_index].token_type == TOKEN_RIGHT_BRACKET){
                // GENERATE substr()
                if (token[token_index - 8].token_type == TOKEN_EQUAL) {
                    gen_call_substr(token, token_index);
                }

                return SYNTAX_OK;
            }
            else if (token[token_index].token_type == TOKEN_COMMA){
                fprintf(stderr, "Error: Invalid parameter count supplied to function \'%s\'\n", "substr()");
                return RC_SEMANTIC_FUNC_PARAM_ERR;
            }
            else {
                err_print(")", token[token_index].token_type);
                return RC_SYN_ERR;
            }

        case TOKEN_FUNCTION_ORD:
            builtin_func_used[4] = true;
            if (built_in_func_type != NULL) {
                *built_in_func_type = token[token_index].token_type;
            }
            if (!unget_token) {
                get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // (
            }
            else {
                unget_token = false;
            }

            if (token[token_index].token_type != TOKEN_LEFT_BRACKET){
                err_print("(", token[token_index].token_type);
                return RC_SYN_ERR;
            }

            if (!unget_token) {
                get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // string or id
            }
            else {
                unget_token = false;
            }

            if (token[token_index].token_type == TOKEN_IDENTIFIER){
                ST_Item *symbol;
                if ((symbol = stack_search(&ptr_scanner->st_stack, &token[token_index].attribute.string_val)) == NULL) {
                    fprintf(stderr, "Error: Undefined variable \'%s\'\n", token[token_index].attribute.string_val.string);
                    return RC_SEMANTIC_IDENTIFIER_ERR;
                } else if (symbol->type != TYPE_STRING) {
                    fprintf(stderr, "Error: Invalid parameter type supplied to function \'%s\'\n", "ord()");
                    return RC_SEMANTIC_FUNC_PARAM_ERR;
                }
            }
            else if (token[token_index].token_type != TOKEN_STRING_LITERAL){
                err_print("string or id", token[token_index].token_type);
                return RC_SEMANTIC_FUNC_PARAM_ERR;
            }

            if (!unget_token) {
                get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // ,
            }
            else {
                unget_token = false;
            }

            if (token[token_index].token_type != TOKEN_COMMA){
                err_print(",", token[token_index].token_type);
                return RC_SYN_ERR;
            }

            if (!unget_token) {
                get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // int or id
            }
            else {
                unget_token = false;
            }

            if (token[token_index].token_type == TOKEN_IDENTIFIER){
                ST_Item *symbol;
                if ((symbol = stack_search(&ptr_scanner->st_stack, &token[token_index].attribute.string_val)) == NULL) {
                    fprintf(stderr, "Error: Undefined variable \'%s\'\n", token[token_index].attribute.string_val.string);
                    return RC_SEMANTIC_IDENTIFIER_ERR;
                } else if (symbol->type != TYPE_INT) {
                    fprintf(stderr, "Error: Invalid parameter type supplied to function \'%s\'\n", "ord()");
                    return RC_SEMANTIC_FUNC_PARAM_ERR;
                }
            }
            else if (token[token_index].token_type != TOKEN_INTEGER_LITERAL){
                err_print("int or id", token[token_index].token_type);
                return RC_SEMANTIC_FUNC_PARAM_ERR;
            }

            if (!unget_token) {
                get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // )
            }
            else {
                unget_token = false;
            }

            if (token[token_index].token_type == TOKEN_RIGHT_BRACKET){
                // GENERATE ord()
                if (token[token_index - 6].token_type == TOKEN_EQUAL) {
                    gen_call_ord(token, token_index);
                }

                return SYNTAX_OK;
            }
            else if (token[token_index].token_type == TOKEN_COMMA){
                fprintf(stderr, "Error: Invalid parameter count supplied to function \'%s\'\n", "ord()");
                return RC_SEMANTIC_FUNC_PARAM_ERR;
            }
            else {
                err_print(")", token[token_index].token_type);
                return RC_SYN_ERR;
            }

        case TOKEN_FUNCTION_CHR:
            builtin_func_used[5] = true;
            if (built_in_func_type != NULL) {
                *built_in_func_type = token[token_index].token_type;
            }
            if (!unget_token) {
                get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // (
            }
            else {
                unget_token = false;
            }

            if (token[token_index].token_type != TOKEN_LEFT_BRACKET){
                err_print("(", token[token_index].token_type);
                return RC_SYN_ERR;
            }

            if (!unget_token) {
                get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // int or id
            }
            else {
                unget_token = false;
            }

            if (token[token_index].token_type == TOKEN_IDENTIFIER){
                ST_Item *symbol;
                if ((symbol = stack_search(&ptr_scanner->st_stack, &token[token_index].attribute.string_val)) == NULL) {
                    fprintf(stderr, "Error: Undefined variable \'%s\'\n", token[token_index].attribute.string_val.string);
                    return RC_SEMANTIC_IDENTIFIER_ERR;
                } else if (symbol->type != TYPE_INT) {
                    fprintf(stderr, "Error: Invalid parameter type supplied to function \'%s\'\n", "chr()");
                    return RC_SEMANTIC_FUNC_PARAM_ERR;
                }
            }
            else if (token[token_index].token_type != TOKEN_INTEGER_LITERAL){
                err_print("int or id", token[token_index].token_type);
                return RC_SEMANTIC_FUNC_PARAM_ERR;
            }

            if (!unget_token) {
                get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // )
            }
            else {
                unget_token = false;
            }

            if (token[token_index].token_type == TOKEN_RIGHT_BRACKET){
                // GENERATE chr()
                if (token[token_index - 4].token_type == TOKEN_EQUAL) {
                    gen_call_chr(token, token_index);
                }
                return SYNTAX_OK;
            } else if (token[token_index].token_type == TOKEN_COMMA){
                fprintf(stderr, "Error: Invalid parameter count supplied to function \'%s\'\n", "chr()");
                return RC_SEMANTIC_FUNC_PARAM_ERR;
            }
            else {
                err_print(")", token[token_index].token_type);
                return RC_SYN_ERR;
            }

        default:
            return RC_SYN_ERR;
    }
}

int expr(scannerT *ptr_scanner, tokenT token[], bool two_tokens, int *result_data_type){
    if (!unget_token) {
        get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // beginning of expr
    }
    else {
        unget_token = false;
    }

    tokenT last_expr_token, expr_result_token;
    token_init(&last_expr_token);
    token_init(&expr_result_token);

    debug_parser("\nEntering expression analysis%s.\n", "");
    if (two_tokens){
        tmp_result = expr_check(&token[token_index - 1], &token[token_index], &last_expr_token,
                                &expr_result_token, ptr_scanner);
        debug_parser("\nExited expression analysis with %d.\n", tmp_result);
        if (tmp_result != SYNTAX_OK)
            return tmp_result;
    }
    else {
        tmp_result = expr_check(NULL, &token[token_index], &last_expr_token, &expr_result_token,
                                ptr_scanner);
        debug_parser("\nExited expression analysis with %d.\n", tmp_result);
        if (tmp_result != SYNTAX_OK)
            return tmp_result;
    }

    // SEMANTIC
    if (result_data_type != NULL) {
        switch (expr_result_token.token_type) {
            case EXPRESSION_INT:
                *result_data_type = TYPE_INT;
                break;
            case EXPRESSION_FLOAT64:
                *result_data_type = TYPE_DECIMAL;
                break;
            case EXPRESSION_STRING:
                *result_data_type = TYPE_STRING;
                break;
            default:
                break;
        }
    }
    // SEMANTIC END

    return SYNTAX_OK;
}

int assign_nofunc(scannerT *ptr_scanner, tokenT token[]){
    if (!unget_token) {
        get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // id, item or expression
    }
    else {
        unget_token = false;
    }

    unget_token = true;

    int item_type;
    tmp_result = literal(ptr_scanner, token, &item_type);
    if (tmp_result == SYNTAX_OK){ // literal was found
        get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // operator or extra token
        if (operator_check(&token[token_index]) || token[token_index].token_type == TOKEN_PLUS ||
            token[token_index].token_type == TOKEN_DIVISION || token[token_index].token_type == TOKEN_DOUBLE_EQUAL ||
            token[token_index].token_type == TOKEN_NOT_EQUAL){

            // operator, call expression analysis
            int expr_result_type;
            unget_token = true;
            tmp_result = expr(ptr_scanner, token, true, &expr_result_type);
            was_expr = true;
            assignment_add_expression(&assignment_check_struct, expr_result_type, NULL);

            // GENERATE
            if (!in_return) {
                if (single_assign) {
                    int i = token_index;
                    while (token[i].token_type != TOKEN_EQUAL && token[i].token_type != TOKEN_COLON_EQUAL) {
                        i--;
                    }
                    if (token[i - 1].token_type != TOKEN_UNDERSCORE) {
                        printf("MOVE LF@%s TF@result_here\n", token[i - 1].attribute.string_val.string);
                    }
                } else {
                    if (left_side_assignments[assigns_right].token_type != TOKEN_UNDERSCORE) {
                        printf("MOVE LF@%s TF@result_here\n",
                               left_side_assignments[assigns_right++].attribute.string_val.string);
                    } else {
                        assigns_right++;
                    }
                }
            }
            // END GENERATE

            return tmp_result;
        }
        else {
            // extra token was read
            unget_token = true;
            assignment_add_expression(&assignment_check_struct, item_type, NULL);

            // GENERATE
            if (!in_return) {
                if (single_assign) {
                    int i = token_index;
                    while (token[i].token_type != TOKEN_EQUAL && token[i].token_type != TOKEN_COLON_EQUAL) {
                        i--;
                    }

                    if (token[i - 1].token_type != TOKEN_UNDERSCORE) {
                        printf("MOVE LF@%s ", token[i - 1].attribute.string_val.string);
                        gen_print_type(&token[token_index - 1]);

                        if (token[token_index - 1].token_type == TOKEN_STRING_LITERAL) {
                            stringT escaped_str;
                            string_init(&escaped_str);
                            gen_escape_string(token[token_index - 1].attribute.string_val.string, &escaped_str);
                            printf("%s\n", escaped_str.string);
                        } else if (token[token_index - 1].token_type == TOKEN_DECIMAL_LITERAL ||
                                   token[token_index - 1].token_type == TOKEN_EXPONENT_LITERAL) {
                            char str_hex_float[1000];
                            sprintf(str_hex_float, "%a",
                                    strtof(token[token_index - 1].attribute.string_val.string, NULL));
                            printf("%s\n", str_hex_float);
                        } else {
                            printf("%s\n", token[token_index - 1].attribute.string_val.string);
                        }
                    }
                } else {
                    if (left_side_assignments[assigns_right].token_type != TOKEN_UNDERSCORE) {
                        printf("MOVE LF@%s ", left_side_assignments[assigns_right++].attribute.string_val.string);
                        gen_print_type(&token[token_index - 1]);

                        if (token[token_index - 1].token_type == TOKEN_STRING_LITERAL) {
                            stringT escaped_str;
                            string_init(&escaped_str);
                            gen_escape_string(token[token_index - 1].attribute.string_val.string, &escaped_str);
                            printf("%s\n", escaped_str.string);
                        } else if (token[token_index - 1].token_type == TOKEN_DECIMAL_LITERAL ||
                                   token[token_index - 1].token_type == TOKEN_EXPONENT_LITERAL) {
                            char str_hex_float[1000];
                            sprintf(str_hex_float, "%a",
                                    strtof(token[token_index - 1].attribute.string_val.string, NULL));
                            printf("%s\n", str_hex_float);
                        } else {
                            printf("%s\n", token[token_index - 1].attribute.string_val.string);
                        }
                    } else {
                        assigns_right++;
                    }
                }
            }
            // END GENERATE

            return SYNTAX_OK;
        }
    }

    unget_token = true;

    if (token[token_index].token_type == TOKEN_IDENTIFIER){
        ST_Item *identifier;
        if ((identifier = stack_search(&ptr_scanner->st_stack, &token[token_index].attribute.string_val)) == NULL) {
            fprintf(stderr, "Error: Undefined variable \'%s\'\n", token[token_index].attribute.string_val.string);
            return RC_SEMANTIC_IDENTIFIER_ERR;
        }

        get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // operator or extra token
        if (operator_check(&token[token_index]) || token[token_index].token_type == TOKEN_PLUS ||
            token[token_index].token_type == TOKEN_DIVISION || token[token_index].token_type == TOKEN_DOUBLE_EQUAL ||
            token[token_index].token_type == TOKEN_NOT_EQUAL){
            // operator, call expression analysis
            int expr_result_type;
            unget_token = true;
            tmp_result = expr(ptr_scanner, token, true, &expr_result_type);
            was_expr = true;
            assignment_add_expression(&assignment_check_struct, expr_result_type, NULL);

            // GENERATE
            if (!in_return) {
                if (single_assign) {
                    int i = token_index;
                    while (token[i].token_type != TOKEN_EQUAL && token[i].token_type != TOKEN_COLON_EQUAL) {
                        i--;
                    }
                    if (token[i - 1].token_type != TOKEN_UNDERSCORE) {
                        printf("MOVE LF@%s TF@result_here\n", token[i - 1].attribute.string_val.string);
                    }
                } else {
                    if (left_side_assignments[assigns_right].token_type != TOKEN_UNDERSCORE) {
                        printf("MOVE LF@%s TF@result_here\n",
                               left_side_assignments[assigns_right++].attribute.string_val.string);
                    } else {
                        assigns_right++;
                    }
                }
            }
            // END GENERATE

            return tmp_result;
        }
        else {
            // extra token was read
            unget_token = true;
            assignment_add_expression(&assignment_check_struct, item_type, identifier);

            // GENERATE
            if (!in_return) {
                if (single_assign) {
                    int i = token_index;
                    while (token[i].token_type != TOKEN_EQUAL && token[i].token_type != TOKEN_COLON_EQUAL) {
                        i--;
                    }
                    if (token[i - 1].token_type != TOKEN_UNDERSCORE) {
                        printf("MOVE LF@%s ", token[i - 1].attribute.string_val.string);
                        gen_print_type(&token[token_index - 1]);
                        printf("%s\n", token[token_index - 1].attribute.string_val.string);
                    }
                } else {
                    if (left_side_assignments[assigns_right].token_type != TOKEN_UNDERSCORE) {
                        printf("MOVE LF@%s ", left_side_assignments[assigns_right++].attribute.string_val.string);
                        gen_print_type(&token[token_index - 1]);
                        printf("%s\n", token[token_index - 1].attribute.string_val.string);
                    } else {
                        assigns_right++;
                    }
                }
            }
            // END GENERATE

            return SYNTAX_OK;
        }
    }

    int expr_result_type;
    tmp_result = expr(ptr_scanner, token, false, &expr_result_type);
    was_expr = true;
    assignment_add_expression(&assignment_check_struct, expr_result_type, NULL);
    return tmp_result;
}

int assign_nofunc_next(scannerT *ptr_scanner, tokenT token[], int return_value_number){
    bool wasnt_expr = false;
    return_value_number++;

    if (was_expr){
        unget_token = true;
        was_expr = false;
    }
    else {
        wasnt_expr = true;
    }

    if (!unget_token) {
        get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // , or extra token was read
    }
    else {
        unget_token = false;
    }

    if (token[token_index].token_type == TOKEN_COMMA){
        // GENERATE
        if (wasnt_expr){
            printf("MOVE LF@%%retval%d ", return_value_number);
            gen_print_type(&token[token_index - 1]);
            if (token[token_index - 1].token_type == TOKEN_STRING_LITERAL) {
                stringT escaped_str;
                string_init(&escaped_str);
                gen_escape_string(token[token_index - 1].attribute.string_val.string, &escaped_str);
                printf("%s\n", escaped_str.string);
            } else if (token[token_index - 1].token_type == TOKEN_DECIMAL_LITERAL ||
                       token[token_index - 1].token_type == TOKEN_EXPONENT_LITERAL) {
                char str_hex_float[1000];
                sprintf(str_hex_float, "%a",
                        strtof(token[token_index - 1].attribute.string_val.string, NULL));
                printf("%s\n", str_hex_float);
            } else {
                printf("%s\n", token[token_index - 1].attribute.string_val.string);
            }

            wasnt_expr = false;
        }
        else {
            printf("MOVE LF@%%retval%d TF@result_here\n", return_value_number);
        }
        // END GENERATE

        tmp_result = assign_nofunc(ptr_scanner, token);
        if (tmp_result != SYNTAX_OK)
            return tmp_result;

        return assign_nofunc_next(ptr_scanner, token, return_value_number);
    }
    else {
        // GENERATE
        if (wasnt_expr){
            printf("MOVE LF@%%retval%d ", return_value_number);
            gen_print_type(&token[token_index - 1]);
            if (token[token_index - 1].token_type == TOKEN_STRING_LITERAL) {
                stringT escaped_str;
                string_init(&escaped_str);
                gen_escape_string(token[token_index - 1].attribute.string_val.string, &escaped_str);
                printf("%s\n", escaped_str.string);
            } else if (token[token_index - 1].token_type == TOKEN_DECIMAL_LITERAL ||
                       token[token_index - 1].token_type == TOKEN_EXPONENT_LITERAL) {
                char str_hex_float[1000];
                sprintf(str_hex_float, "%a",
                        strtof(token[token_index - 1].attribute.string_val.string, NULL));
                printf("%s\n", str_hex_float);
            } else {
                printf("%s\n", token[token_index - 1].attribute.string_val.string);
            }

            wasnt_expr = false;
        }
        else {
            printf("MOVE LF@%%retval%d TF@result_here\n", return_value_number);
        }
        // END GENERATE

        unget_token = true;
        return SYNTAX_OK;
    }
}

int assign_nofunc_list(scannerT *ptr_scanner, tokenT token[]){
    int return_value_number = 0;
    tmp_result = assign_nofunc(ptr_scanner, token);

    if (was_expr){
        unget_token = true;
    }

    if (tmp_result != SYNTAX_OK)
        return tmp_result;

    return assign_nofunc_next(ptr_scanner, token, return_value_number);
}

int assign(scannerT *ptr_scanner, tokenT token[], bool *skip_sides_semantic_type_check){
    if (!unget_token) {
        get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // id, item, built-in function or expr
    }
    else {
        unget_token = false;
    }

    unget_token = true;

    int item_type;
    tmp_result = literal(ptr_scanner, token, &item_type);
    if (tmp_result == SYNTAX_OK){ // literal was found
        get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // operator or extra token

        if (operator_check(&token[token_index]) || token[token_index].token_type == TOKEN_PLUS ||
            token[token_index].token_type == TOKEN_DIVISION || token[token_index].token_type == TOKEN_DOUBLE_EQUAL ||
            token[token_index].token_type == TOKEN_NOT_EQUAL){
            // operator, call expression analysis
            int expr_result_type;
            unget_token = true;
            tmp_result = expr(ptr_scanner, token, true, &expr_result_type);
            was_expr = true;
            assignment_add_expression(&assignment_check_struct, expr_result_type, NULL);

            // GENERATE
            if (!in_return) {
                if (single_assign) {
                    int i = token_index;
                    while (token[i].token_type != TOKEN_EQUAL && token[i].token_type != TOKEN_COLON_EQUAL) {
                        i--;
                    }
                    if (token[i - 1].token_type != TOKEN_UNDERSCORE) {
                        printf("MOVE LF@%s TF@result_here\n", token[i - 1].attribute.string_val.string);
                    }
                } else {
                    if (left_side_assignments[assigns_right].token_type != TOKEN_UNDERSCORE) {
                        printf("MOVE LF@%s TF@result_here\n",
                               left_side_assignments[assigns_right++].attribute.string_val.string);
                    } else {
                        assigns_right++;
                    }
                }
            }
            // END GENERATE

            return tmp_result;
        }
        else {
            // extra token was read
            unget_token = true;
            assignment_add_expression(&assignment_check_struct, item_type, NULL);

            // GENERATE
            if (!in_return) {
                if (single_assign) {
                    int i = token_index;
                    while (token[i].token_type != TOKEN_EQUAL && token[i].token_type != TOKEN_COLON_EQUAL) {
                        i--;
                    }
                    if (token[i - 1].token_type != TOKEN_UNDERSCORE) {
                        printf("MOVE LF@%s ", token[i - 1].attribute.string_val.string);
                        gen_print_type(&token[token_index - 1]);

                        if (token[token_index - 1].token_type == TOKEN_STRING_LITERAL) {
                            stringT escaped_str;
                            string_init(&escaped_str);
                            gen_escape_string(token[token_index - 1].attribute.string_val.string, &escaped_str);
                            printf("%s\n", escaped_str.string);
                        } else if (token[token_index - 1].token_type == TOKEN_DECIMAL_LITERAL ||
                                   token[token_index - 1].token_type == TOKEN_EXPONENT_LITERAL) {
                            char str_hex_float[1000];
                            sprintf(str_hex_float, "%a",
                                    strtof(token[token_index - 1].attribute.string_val.string, NULL));
                            printf("%s\n", str_hex_float);
                        } else {
                            printf("%s\n", token[token_index - 1].attribute.string_val.string);
                        }
                    }
                } else {
                    if (left_side_assignments[assigns_right].token_type != TOKEN_UNDERSCORE) {
                        printf("MOVE LF@%s ", left_side_assignments[assigns_right++].attribute.string_val.string);
                        gen_print_type(&token[token_index - 1]);

                        if (token[token_index - 1].token_type == TOKEN_STRING_LITERAL) {
                            stringT escaped_str;
                            string_init(&escaped_str);
                            gen_escape_string(token[token_index - 1].attribute.string_val.string, &escaped_str);
                            printf("%s\n", escaped_str.string);
                        } else if (token[token_index - 1].token_type == TOKEN_DECIMAL_LITERAL ||
                                   token[token_index - 1].token_type == TOKEN_EXPONENT_LITERAL) {
                            char str_hex_float[1000];
                            sprintf(str_hex_float, "%a",
                                    strtof(token[token_index - 1].attribute.string_val.string, NULL));
                            printf("%s\n", str_hex_float);
                        } else {
                            printf("%s\n", token[token_index - 1].attribute.string_val.string);
                        }
                    } else {
                        assigns_right++;
                    }
                }
            }
            // END GENERATE

            return SYNTAX_OK;
        }
    }

    unget_token = true;

    int built_in_func_type_token;
    tmp_result = builtin_func(ptr_scanner, token, &built_in_func_type_token);
    if (tmp_result == SYNTAX_OK) {
        assignment_add_built_in_function(&assignment_check_struct, built_in_func_type_token);
        return SYNTAX_OK;
    } else if (tmp_result != RC_SYN_ERR) {
        return tmp_result;
    }

    if (token[token_index].token_type == TOKEN_IDENTIFIER){
        // check following token, if (, then function parameter list follows, otherwise an extra token was read
        if (!unget_token) {
            get_next_token(ptr_scanner, &token[++token_index], OPTIONAL);
        }
        else {
            unget_token = false;
        }

        if (token[token_index].token_type == TOKEN_LEFT_BRACKET) {
            if (skip_sides_semantic_type_check != NULL) {
                *skip_sides_semantic_type_check = true;
            }

            // GENERATE
            printf("\n");
            gen_createframe();
            // END GENERATE

            late_check_stack_push_method(&semantic_late_check_stack, &token[token_index-1].attribute.string_val);
            late_check_stack_item_create_assignment_list(semantic_late_check_stack.top,
                                                         assignment_check_struct.left_side_types_list_first);
            return id_list1(ptr_scanner, token, true);
        }
        else {
            ST_Item *identifier;
            if ((identifier = stack_search(&ptr_scanner->st_stack,
                                           &token[token_index-1].attribute.string_val)) == NULL) {
                fprintf(stderr, "Error: Undefined variable \'%s\'\n", token[token_index-1].attribute.string_val.string);
                return RC_SEMANTIC_IDENTIFIER_ERR;
            }
            int expr_result_type;
            unget_token = true;
            tmp_result = expr(ptr_scanner, token, true, &expr_result_type);
            was_expr = true;
            assignment_add_expression(&assignment_check_struct, expr_result_type, identifier);

            // GENERATE
            if (!in_return) {
                if (single_assign) {
                    int i = token_index;
                    while (token[i].token_type != TOKEN_EQUAL && token[i].token_type != TOKEN_COLON_EQUAL) {
                        i--;
                    }
                    if (token[i - 1].token_type != TOKEN_UNDERSCORE) {
                        printf("MOVE LF@%s TF@result_here\n", token[i - 1].attribute.string_val.string);
                    }
                } else {
                    if (left_side_assignments[assigns_right].token_type != TOKEN_UNDERSCORE) {
                        printf("MOVE LF@%s TF@result_here\n",
                               left_side_assignments[assigns_right].attribute.string_val.string);
                    } else {
                        assigns_right++;
                    }
                }
            }
            // END GENERATE

            return tmp_result;
        }
    }

    int expr_result_type;
    tmp_result = expr(ptr_scanner, token, false, &expr_result_type);
    was_expr = true;
    assignment_add_expression(&assignment_check_struct, expr_result_type, NULL);
    return tmp_result;
}

int assign_next(scannerT *ptr_scanner, tokenT token[], bool *skip_sides_semantic_type_check){
    if (!was_expr){
        if (!unget_token) {
            get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // command or }
        }
        else {
            unget_token = false;
        }
    }
    else {
        was_expr = false;
    }

    if (token[token_index].token_type == TOKEN_COMMA){
        tmp_result = assign(ptr_scanner, token, NULL);
        if (tmp_result != SYNTAX_OK)
            return tmp_result;

        return assign_next(ptr_scanner, token, skip_sides_semantic_type_check);
    }
    else {
        unget_token = true;
        return SYNTAX_OK;
    }
}

int assign_list(scannerT *ptr_scanner, tokenT token[], bool *skip_sides_semantic_type_check){
    tmp_result = assign(ptr_scanner, token, skip_sides_semantic_type_check);
    if (tmp_result != SYNTAX_OK)
        return tmp_result;

    return assign_next(ptr_scanner, token, skip_sides_semantic_type_check);
}

int id_next2(scannerT *ptr_scanner, tokenT token[], int id_number){
    if (!unget_token) {
        get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // = or ,
    }
    else {
        unget_token = false;
    }

    if (token[token_index].token_type == TOKEN_EQUAL) {
        return SYNTAX_OK;
    }
    else if (token[token_index].token_type == TOKEN_COMMA){
        tmp_result = id(ptr_scanner, token, true);
        if (tmp_result == SYNTAX_OK) {
            // GENERATE
            id_number++;
            left_side_assignments[id_number] = token[token_index];
            // END GENERATE

            return id_next2(ptr_scanner, token, id_number);
        }
        else {
            err_print("id or _", token[token_index].token_type);
            return tmp_result;
        }
    }
    else {
        err_print(", or =", token[token_index].token_type);
        return RC_SYN_ERR;
    }
}

int id_list2(scannerT *ptr_scanner, tokenT token[]){
    if (!unget_token) {
        get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // id or _
    }
    else {
        unget_token = false;
    }

    int id_number = 1;
    assigns_right = 0;

    unget_token = true; // id or _ is already loaded
    tmp_result = id(ptr_scanner, token, true);
    if (tmp_result == SYNTAX_OK) {
        // GENERATE
        left_side_assignments[id_number] = token[token_index];
        // END GENERATE

        return id_next2(ptr_scanner, token, id_number);
    }
    else {
        err_print("id, _ or =", token[token_index].token_type);
        return tmp_result;
    }
}

int id_next1(scannerT *ptr_scanner, tokenT token[], int param_num, bool assignment){
    if (!unget_token) {
        get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // ) or ,
    }
    else {
        unget_token = false;
    }
    ST_Item *identifier = NULL;
    int item_type;

    switch (token[token_index].token_type){
        case TOKEN_RIGHT_BRACKET:
            // GENERATE
            printf("CALL $%s\n", token[token_index - 1 - 2 * param_num].attribute.string_val.string);

            if (assignment) {
                if (single_assign) {
                    int i = token_index;
                    while (token[i].token_type != TOKEN_EQUAL) {
                        i--;
                    }
                    printf("MOVE LF@%s TF@%%retval%d\n", token[i - 1].attribute.string_val.string, param_num);
                }
                else {
                    int i = 0;
                    while (left_side_assignments[i].token_type != TOKEN_EMPTY){
                        printf("MOVE LF@%s TF@%%retval%d\n", left_side_assignments[i].attribute.string_val.string,
                               i + 1);
                        i++;
                    }
                }
            }
            // END GENERATE

            return SYNTAX_OK;

        case TOKEN_COMMA:
            param_num++;

            if (!unget_token) {
                get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); //  id or literal
            }
            else {
                unget_token = false;
            }

            if (token[token_index].token_type == TOKEN_UNDERSCORE) {
                fprintf(stderr, "Error: Variable \'_\' cannot be used as function parameter.");
                return RC_SEMANTIC_FUNC_PARAM_ERR;
            }

            if (token[token_index].token_type != TOKEN_IDENTIFIER){
                unget_token = true;
                tmp_result = literal(ptr_scanner, token, &item_type);
                if (tmp_result == SYNTAX_OK){
                    // GENERATE - create temporary variable for parameter
                    if (in_for){
                        char for_defvar_str[100];
                        sprintf(for_defvar_str, "DEFVAR TF@%%%d\n", param_num);
                        string_add_string(&defvars_in_for, for_defvar_str);
                    }
                    else {
                        printf("DEFVAR TF@%%%d\n", param_num);
                    }
                    gen_parameter(&token[token_index], param_num);

                    late_check_stack_item_add_parameter(semantic_late_check_stack.top, item_type);
                    return id_next1(ptr_scanner, token, param_num, assignment);
                }
                else {
                    err_print("id or literal", token[token_index].token_type);
                    return RC_SYN_ERR;
                }
            }

            // GENERATE - create temporary variable for parameter
            if (in_for){
                char for_defvar_str[100];
                sprintf(for_defvar_str, "DEFVAR TF@%%%d\n", param_num);
                string_add_string(&defvars_in_for, for_defvar_str);
            }
            else {
                printf("DEFVAR TF@%%%d\n", param_num);
            }
            gen_parameter(&token[token_index], param_num);

            if ((identifier = stack_search(&ptr_scanner->st_stack, &token[token_index].attribute.string_val)) == NULL) {
                fprintf(stderr, "Error: Undefined variable \'%s\'\n", token[token_index].attribute.string_val.string);
                return RC_SEMANTIC_IDENTIFIER_ERR;
            }
            late_check_stack_item_add_parameter(semantic_late_check_stack.top, identifier->type);

            return id_next1(ptr_scanner, token, param_num, assignment);

        default:
            err_print(") or ,", token[token_index].token_type);
            return RC_SYN_ERR;
    }
}

int id_list1(scannerT *ptr_scanner, tokenT token[], bool assignment){
    if (!unget_token) {
        get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // id, literal or )
    }
    else {
        unget_token = false;
    }
    ST_Item *identifier = NULL;
    int item_type, param_num = 1;

    switch (token[token_index].token_type){
        case TOKEN_RIGHT_BRACKET:
            // GENERATE
            printf("CALL $%s\n", token[token_index - 2].attribute.string_val.string);

            if (assignment) {
                if (single_assign) {
                    int i = token_index;
                    while (token[i].token_type != TOKEN_EQUAL) {
                        i--;
                    }
                    printf("MOVE LF@%s TF@%%retval%d\n", token[i - 1].attribute.string_val.string, param_num);
                }
                else {
                    int i = 0;
                    while (left_side_assignments[i].token_type != TOKEN_EMPTY){
                        printf("MOVE LF@%s TF@%%retval%d\n", left_side_assignments[i].attribute.string_val.string,
                               i + 1);
                        i++;
                    }
                }
            }
            // END GENERATE
            return SYNTAX_OK;

        case TOKEN_UNDERSCORE:
            fprintf(stderr, "Error: Variable \'_\' cannot be used as function parameter.");
            return RC_SEMANTIC_FUNC_PARAM_ERR;

        case TOKEN_IDENTIFIER:
            if ((identifier = stack_search(&ptr_scanner->st_stack, &token[token_index].attribute.string_val)) == NULL) {
                fprintf(stderr, "Error: Undefined variable \'%s\'\n", token[token_index].attribute.string_val.string);
                return RC_SEMANTIC_IDENTIFIER_ERR;
            }
            late_check_stack_item_add_parameter(semantic_late_check_stack.top, identifier->type);

            // GENERATE - create temporary variable for parameter
            if (in_for){
                char for_defvar_str[100];
                sprintf(for_defvar_str, "DEFVAR TF@%%%d\n", param_num);
                string_add_string(&defvars_in_for, for_defvar_str);
            }
            else {
                printf("DEFVAR TF@%%%d\n", param_num);

            }
            gen_parameter(&token[token_index], param_num);

            return id_next1(ptr_scanner, token, param_num, assignment);

        default:
            unget_token = true;
            tmp_result = literal(ptr_scanner, token, &item_type);
            if (tmp_result == SYNTAX_OK){
                // GENERATE - create temporary variable for parameter
                if (in_for){
                    char for_defvar_str[100];
                    sprintf(for_defvar_str, "DEFVAR TF@%%%d\n", param_num);
                    string_add_string(&defvars_in_for, for_defvar_str);
                }
                else {
                    printf("DEFVAR TF@%%%d\n", param_num);

                }
                gen_parameter(&token[token_index], param_num);

                late_check_stack_item_add_parameter(semantic_late_check_stack.top, item_type);
                return id_next1(ptr_scanner, token, param_num, assignment);
            }
            else {
                err_print("id, literal or )", token[token_index].token_type);
                return RC_SYN_ERR;
            }
    }
}

int underscore_command(scannerT *ptr_scanner, tokenT token[]){
    ST_Item *symbol = NULL;

    bool proceed_semantic_check = false;

    if (!unget_token) {
        get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // = or ,
    }
    else {
        unget_token = false;
    }

    switch (token[token_index].token_type){
        case TOKEN_EQUAL:
            // SEMANTIC
            assignment_add_identifier(&assignment_check_struct, token[token_index-1].token_type, symbol);
            // END SEMANTIC

            single_assign = true;
            tmp_result = assign(ptr_scanner, token, &proceed_semantic_check);
            single_assign = false;

            if (tmp_result == SYNTAX_OK && !proceed_semantic_check) {
                return compare_left_right_params(&assignment_check_struct);
            }
            return tmp_result;

        case TOKEN_COMMA:
            // SEMANTIC
            assignment_add_identifier(&assignment_check_struct, token[token_index-1].token_type, symbol);
            //END SEMANTIC

            // GENERATE
            for (int i = 0; i < LEFT_SIDE_TOKEN_COUNT; i++){
                token_clear(&left_side_assignments[i]);
            }

            left_side_assignments[0] = token[token_index - 1];
            // END GENERATE

            tmp_result = id_list2(ptr_scanner, token);
            if (tmp_result != SYNTAX_OK)
                return tmp_result;

            tmp_result = assign_list(ptr_scanner, token, &proceed_semantic_check);
            if (tmp_result == SYNTAX_OK && !proceed_semantic_check) {
                return compare_left_right_params(&assignment_check_struct);
            }
            return tmp_result;

        default:
            err_print("= or ,", token[token_index].token_type);
            return RC_SYN_ERR;
    }
}

int id_command(scannerT *ptr_scanner, tokenT token[]){
    Symtable *ptr_curr_scope_sym_table = stack_top(&ptr_scanner->st_stack).symtable;
    ST_Item *symbol = NULL;

    bool proceed_semantic_check = false;
    ST_Item *function_symb_check;

    if (!unget_token) {
        get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // :=, =, ( or ,
    }
    else {
        unget_token = false;
    }

    switch (token[token_index].token_type){
        case TOKEN_COLON_EQUAL:
            // SEMANTIC: Insert identifier to symtable
            symbol = st_insert_symbol(ptr_curr_scope_sym_table, &token[token_index-1].attribute.string_val,
                                      false);
            assignment_add_identifier(&assignment_check_struct, token[token_index-1].token_type, symbol);
            // END SEMANTIC

            // GENERATE
            if (in_for){
                char for_defvar_str[1000];
                sprintf(for_defvar_str, "DEFVAR LF@%s\n", token[token_index-1].attribute.string_val.string);
                string_add_string(&defvars_in_for, for_defvar_str);
            }
            else {
                gen_defvar_lf(token[token_index-1].attribute.string_val.string);
            }

            single_assign = true;
            tmp_result = assign_nofunc(ptr_scanner, token);
            single_assign = false;

            if (was_expr){
                unget_token = true;
                was_expr = false;
            }
            if (tmp_result == SYNTAX_OK) {
                return assignment_derive_id_type(&assignment_check_struct);
            }

            return tmp_result;
        
        case TOKEN_EQUAL:
            // SEMANTIC: Check if identifier exists in symtable
            if (string_compare_constant(&token[token_index-1].attribute.string_val, "_") != 0) {
                if ((symbol = stack_search(&ptr_scanner->st_stack,
                                           &token[token_index-1].attribute.string_val)) == NULL) {
                    fprintf(stderr, "Error: Undefined variable \'%s\'\n",
                            token[token_index-1].attribute.string_val.string);
                    return RC_SEMANTIC_IDENTIFIER_ERR;
                }
            }
            assignment_add_identifier(&assignment_check_struct, token[token_index-1].token_type, symbol);
            // END SEMANTIC

            single_assign = true;
            tmp_result = assign(ptr_scanner, token, &proceed_semantic_check);
            single_assign = false;

            if (tmp_result == SYNTAX_OK && !proceed_semantic_check) {
                return compare_left_right_params(&assignment_check_struct);
            }
            return tmp_result;

        case TOKEN_LEFT_BRACKET:
            gen_createframe();
            late_check_stack_push_method(&semantic_late_check_stack, &token[token_index-1].attribute.string_val);

            if ((function_symb_check = st_search(ptr_scanner->st_stack.top->symtable,
                                                 &token[token_index-1].attribute.string_val)) != NULL &&
                                                 !st_item_is_function(function_symb_check)) {
                fprintf(stderr, "Variable \'%s\' is not a function",
                        token[token_index-1].attribute.string_val.string);
                return RC_SEMANTIC_IDENTIFIER_ERR;
            }

            return id_list1(ptr_scanner, token, false);

        case TOKEN_COMMA:
            // SEMANTIC
            if (string_compare_constant(&token[token_index-1].attribute.string_val, "_") != 0) {
                if ((symbol = stack_search(&ptr_scanner->st_stack,
                                           &token[token_index-1].attribute.string_val)) == NULL) {
                    fprintf(stderr, "Error: Undefined variable \'%s\'\n",
                            token[token_index-1].attribute.string_val.string);
                    return RC_SEMANTIC_IDENTIFIER_ERR;
                }
            }
            assignment_add_identifier(&assignment_check_struct, token[token_index-1].token_type, symbol);
            //END SEMANTIC

            // GENERATE
            for (int i = 0; i < LEFT_SIDE_TOKEN_COUNT; i++){
                token_clear(&left_side_assignments[i]);
            }

            left_side_assignments[0] = token[token_index - 1];
            // END GENERATE

            tmp_result = id_list2(ptr_scanner, token);
            if (tmp_result != SYNTAX_OK)
                return tmp_result;

            tmp_result = assign_list(ptr_scanner, token, &proceed_semantic_check);
            if (tmp_result == SYNTAX_OK && !proceed_semantic_check) {
                return compare_left_right_params(&assignment_check_struct);
            }
            return tmp_result;

        default:
            err_print(":=, =, ( or ,", token[token_index].token_type);
            return RC_SYN_ERR;
    }
}

int cycle_assign(scannerT *ptr_scanner, tokenT token[], int for_count){
    if (!unget_token) {
        get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // id, _ or {
    }
    else {
        unget_token = false;
    }

    if (token[token_index].token_type == TOKEN_LEFT_CURLY_BRACE){
        stack_push(&ptr_scanner->st_stack);
        indent_level++;

        // GENERATE
        printf("\nLABEL $for%d$assign\n", for_count);
        printf("JUMP $for%d$start\n", for_count);

        return SYNTAX_OK;
    }
    else {
        unget_token = true; // id or _ is already loaded
        tmp_result = id(ptr_scanner, token, true);
        if (tmp_result == SYNTAX_OK){
            if (!unget_token) {
                get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // =
            }
            else {
                unget_token = false;
            }

            if (token[token_index].token_type != TOKEN_EQUAL){
                err_print("=", token[token_index].token_type);
                return RC_SYN_ERR;
            }

            // GENERATE
            printf("\nLABEL $for%d$assign\n", for_count);

            int expr_result_type;
            tmp_result = expr(ptr_scanner, token, false, &expr_result_type);
            if (tmp_result != SYNTAX_OK) {
                return tmp_result;
            }

            assignment_add_expression(&assignment_check_struct, expr_result_type, NULL);

            if (token[token_index].token_type != TOKEN_LEFT_CURLY_BRACE){
                err_print("{", token[token_index].token_type);
                return RC_SYN_ERR;
            }

            // GENERATE
            int i = token_index;
            while (token[i].token_type != TOKEN_EQUAL){
                i--;
            }
            printf("MOVE LF@%s TF@result_here\n", token[i - 1].attribute.string_val.string);
            printf("JUMP $for%d$start\n", for_count);

            indent_level++;
            stack_push(&ptr_scanner->st_stack);
            return compare_left_right_params(&assignment_check_struct); // return SYNTAX_OK
        }
        else {
            err_print("id, _ or {", token[token_index].token_type);
            return RC_SYN_ERR;
        }
    }
}

int cycle_init(scannerT *ptr_scanner, tokenT token[]){
    if (!unget_token) {
        get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // id or ;
    }
    else {
        unget_token = false;
    }

    if (token[token_index].token_type == TOKEN_SEMICOLON){
        return SYNTAX_OK;
    }
    else if (token[token_index].token_type == TOKEN_IDENTIFIER){
        ST_Item *symbol = st_insert_symbol(stack_top(&ptr_scanner->st_stack).symtable,
                                           &token[token_index].attribute.string_val, false);
        assignment_add_identifier(&assignment_check_struct, token[token_index-1].token_type, symbol);
        if (!unget_token) {
            get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // :=
        }
        else {
            unget_token = false;
        }

        if (token[token_index].token_type != TOKEN_COLON_EQUAL){
            err_print(":=", token[token_index].token_type);
            return RC_SYN_ERR;
        }

        // GENERATE
        if (in_for){
            char for_defvar_str[1000];
            sprintf(for_defvar_str, "DEFVAR LF@%s\n", token[token_index-1].attribute.string_val.string);
            string_add_string(&defvars_in_for, for_defvar_str);
        }
        else {
            gen_defvar_lf(token[token_index-1].attribute.string_val.string);
        }

        single_assign = true;
        tmp_result = assign_nofunc(ptr_scanner, token);
        single_assign = false;

        if (tmp_result != SYNTAX_OK) {
            return tmp_result;
        } else {
            assignment_derive_id_type(&assignment_check_struct);
        }

        if (!was_expr){
            if (!unget_token) {
                get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // command or }
            }
            else {
                unget_token = false;
            }
        }
        else {
            was_expr = false;
        }

        if (token[token_index].token_type == TOKEN_SEMICOLON){
            return SYNTAX_OK;
        }
        else {
            err_print(";", token[token_index].token_type);
            return RC_SYN_ERR;
        }
    }
    else {
        err_print("id or ;", token[token_index].token_type);
        return RC_SYN_ERR;
    }
}

int command(scannerT *ptr_scanner, tokenT token[]){
    assignment_struct_empty(&assignment_check_struct);

    // Necessary variables for correct label generating inside conditions and cycles
    static int if_count = 0;
    static int for_count = 0;
    static int for_defvar_label_count = 1;

    static intStack if_stack;
    static intStack for_stack;

    static bool if_stack_initiated = false;
    static bool for_stack_initiated = false;

    switch (token[token_index].token_type){
        case TOKEN_IDENTIFIER:
            return id_command(ptr_scanner, token);

        case TOKEN_UNDERSCORE:
            return underscore_command(ptr_scanner, token);

        case TOKEN_KEYWORD_IF:
            if_count++;

            if (!if_stack_initiated) {
                int_stack_init(&if_stack);
                if_stack_initiated = true;
            }
            int_stack_push(&if_stack, if_count);

            tmp_result = expr(ptr_scanner, token, false, NULL);
            if (tmp_result != SYNTAX_OK)
                return tmp_result;

            if (token[token_index].token_type != TOKEN_LEFT_CURLY_BRACE){
                err_print("{", token[token_index].token_type);
                return RC_SYN_ERR;
            }

            // GENERATE
            indent_level++;
            printf("JUMPIFEQ $if%d$else TF@result_here bool@false\n", int_stack_top(&if_stack));
            // GENERATE END

            stack_push(&ptr_scanner->st_stack);

            tmp_result = command_list(ptr_scanner, token);
            if (tmp_result != SYNTAX_OK)
                return tmp_result;

            if (!unget_token) {
                get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // else
            }
            else {
                unget_token = false;
            }

            if (token[token_index].token_type != TOKEN_KEYWORD_ELSE){
                err_print("else keyword", token[token_index].token_type);
                return RC_SYN_ERR;
            }

            // GENERATE
            printf("JUMP $if%d$end\n", int_stack_top(&if_stack));
            printf("LABEL $if%d$else\n", int_stack_top(&if_stack));

            if (!unget_token) {
                get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // {
            }
            else {
                unget_token = false;
            }

            if (token[token_index].token_type != TOKEN_LEFT_CURLY_BRACE){
                err_print("{", token[token_index].token_type);
                return RC_SYN_ERR;
            }
            indent_level++;
            stack_push(&ptr_scanner->st_stack);

            tmp_result = command_list(ptr_scanner, token);
            if (tmp_result != SYNTAX_OK)
                return tmp_result;

            // GENERATE
            printf("LABEL $if%d$end\n\n", int_stack_top(&if_stack));
            int_stack_pop(&if_stack);
            if (int_stack_empty(&if_stack)){
                int_stack_free(&if_stack);
                if_stack_initiated = false;
            }
            // GENERATE

            return SYNTAX_OK;

        case TOKEN_KEYWORD_FOR:
            if (!for_stack_initiated){
                int_stack_init(&for_stack);
                for_stack_initiated = true;
                in_for = true;

                printf("JUMP $for$defvar%d$start\n", for_defvar_label_count);
                printf("LABEL $for$defvar%d$end\n", for_defvar_label_count);

                char tmp_for_str[100];
                sprintf(tmp_for_str, "LABEL $for$defvar%d$start\n", for_defvar_label_count);
                string_add_string(&defvars_in_for, tmp_for_str);
            }

            for_count++;
            int_stack_push(&for_stack, for_count);
            printf("\n# FOR cycle\n");

            stack_push(&ptr_scanner->st_stack);
            tmp_result = cycle_init(ptr_scanner, token);
            if (tmp_result != SYNTAX_OK)
                return tmp_result;

            // GENERATE
            printf("\nLABEL $for%d$start\n", int_stack_top(&for_stack));

            tmp_result = expr(ptr_scanner, token, false, NULL);
            if (tmp_result != SYNTAX_OK)
                return tmp_result;

            if (token[token_index].token_type != TOKEN_SEMICOLON){
                err_print(";", token[token_index].token_type);
                return RC_SYN_ERR;
            }

            printf("JUMPIFEQ $for%d$end TF@result_here bool@false\n", int_stack_top(&for_stack));
            printf("JUMP $for%d$body\n", int_stack_top(&for_stack));

            tmp_result = cycle_assign(ptr_scanner, token, int_stack_top(&for_stack));
            if (tmp_result != SYNTAX_OK)
                return tmp_result;

            printf("\nLABEL $for%d$body\n", int_stack_top(&for_stack));
            tmp_result = command_list(ptr_scanner, token);
            if (tmp_result == SYNTAX_OK) {
                stack_pop(&ptr_scanner->st_stack);
            }

            // GENERATE
            printf("JUMP $for%d$assign\n", int_stack_top(&for_stack));
            printf("\nLABEL $for%d$end\n\n", int_stack_top(&for_stack));

            int_stack_pop(&for_stack);
            if (int_stack_empty(&for_stack)){
                int_stack_free(&for_stack);
                for_stack_initiated = false;
                in_for = false;

                char tmp_for_str[100];
                sprintf(tmp_for_str, "JUMP $for$defvar%d$end\n", for_defvar_label_count);
                string_add_string(&defvars_in_for, tmp_for_str);

                for_defvar_label_count++;
            }
            //END GENERATE

            return tmp_result;

        case TOKEN_KEYWORD_RETURN:
            // a function can return zero or more values
            in_return = true;

            if (!unget_token) {
                get_next_token(ptr_scanner, &token[++token_index], OPTIONAL);  // } or assignment(s)
            }
            else {
                unget_token = false;
            }

            unget_token = true;
            if (token[token_index].token_type == TOKEN_RIGHT_CURLY_BRACE){
                in_return = false;
                return SYNTAX_OK;
            }
            else {
                printf("\n# return values\n");
                tmp_result = assign_nofunc_list(ptr_scanner, token);
                in_return = false;
                return tmp_result;
            }

        default:
            break;
    }

    unget_token = true;

    int built_in_func_type_token;
    tmp_result = builtin_func(ptr_scanner, token, &built_in_func_type_token);
    if (tmp_result == SYNTAX_OK) {
        assignment_add_built_in_function(&assignment_check_struct, built_in_func_type_token);
        return SYNTAX_OK;
    }

    return tmp_result;
}

int command_list(scannerT *ptr_scanner, tokenT token[]){
    if (!was_expr){
        if (!unget_token) {
            get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // command or }
        }
        else {
            unget_token = false;
        }
    }
    else {
        was_expr = false;
    }

    if (token[token_index].token_type == TOKEN_RIGHT_CURLY_BRACE){
        stack_pop(&ptr_scanner->st_stack);

        indent_level--;

        return SYNTAX_OK;
    }
    else {
        tmp_result = command(ptr_scanner, token);
        if (tmp_result != SYNTAX_OK)
            return tmp_result;

        return command_list(ptr_scanner, token);
    }
}

int return_type(scannerT *ptr_scanner, tokenT token[], ST_Item *ptr_curr_symbol, int return_type_num){
    if (!unget_token) {
        get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // , or )
    }
    else {
        unget_token = false;
    }

    if (token[token_index].token_type == TOKEN_RIGHT_BRACKET){
        if (!unget_token) {
            get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // {
        }
        else {
            unget_token = false;
        }

        if (token[token_index].token_type == TOKEN_LEFT_CURLY_BRACE){
            return SYNTAX_OK;
        }
        else {
            err_print("{", token[token_index].token_type);
            return RC_SYN_ERR;
        }
    }
    else if (token[token_index].token_type == TOKEN_COMMA){
        int item_type;
        tmp_result = item(ptr_scanner, token, &item_type);
        if (tmp_result != SYNTAX_OK) {
            return tmp_result;
        }
        // GENERATE
        return_type_num++;
        if (in_for){
            char for_defvar_str[1000];
            sprintf(for_defvar_str, "DEFVAR LF@%%retval%d\n", return_type_num);
            string_add_string(&defvars_in_for, for_defvar_str);
        }
        else {
            printf("DEFVAR LF@%%retval%d\n", return_type_num);
        }

        printf("MOVE LF@%%retval%d nil@nil\n", return_type_num);

        st_add_function_return_type(ptr_curr_symbol, item_type);

        return return_type(ptr_scanner, token, ptr_curr_symbol, return_type_num);
    }
    else {
        err_print(", or }", token[token_index].token_type);
        return RC_SYN_ERR;
    }
}

int return_type_list(scannerT *ptr_scanner, tokenT token[], ST_Item *ptr_curr_symbol){
    if (!unget_token) {
        get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // ( or {
    }
    else {
        unget_token = false;
    }

    if (token[token_index].token_type == TOKEN_LEFT_CURLY_BRACE){
        return SYNTAX_OK;
    }
    else if (token[token_index].token_type != TOKEN_LEFT_BRACKET){
        err_print("( or {", token[token_index].token_type);
        return RC_SYN_ERR;
    }

    if (!unget_token) {
        get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // ) or item
    }
    else {
        unget_token = false;
    }

    int return_type_num = 1;

    if (token[token_index].token_type == TOKEN_RIGHT_BRACKET){
        if (!unget_token) {
            get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // {
        }
        else {
            unget_token = false;
        }

        if (token[token_index].token_type == TOKEN_LEFT_CURLY_BRACE){
            return SYNTAX_OK;
        }
        else {
            err_print("{", token[token_index].token_type);
            return RC_SYN_ERR;
        }
    }
    else {  // item received
        unget_token = true;
        int item_type;
        tmp_result = item(ptr_scanner, token, &item_type);
        if (tmp_result != SYNTAX_OK) {
            return tmp_result;
        }
        // GENERATE
        printf("# init return values\n");

        if (in_for){
            char for_defvar_str[1000];
            sprintf(for_defvar_str, "DEFVAR LF@%%retval%d\n", return_type_num);
            string_add_string(&defvars_in_for, for_defvar_str);
        }
        else {
            printf("DEFVAR LF@%%retval%d\n", return_type_num);
        }

        printf("MOVE LF@%%retval%d nil@nil\n", return_type_num);

        st_add_function_return_type(ptr_curr_symbol, item_type);

        return return_type(ptr_scanner, token, ptr_curr_symbol, return_type_num);
    }
}

int param(scannerT *ptr_scanner, tokenT token[], ST_Item *ptr_curr_symbol, int param_number){
    if (!unget_token) {
        get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // , or )
    }
    else {
        unget_token = false;
    }

    if (token[token_index].token_type == TOKEN_RIGHT_BRACKET){
        printf("\n");
        return SYNTAX_OK;
    }
    else if (token[token_index].token_type != TOKEN_COMMA){
        err_print(", or )", token[token_index].token_type);
        return RC_SYN_ERR;
    }

    if (!unget_token) {
        get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // function parameter (ID)
    }
    else {
        unget_token = false;
    }

    if (token[token_index].token_type == TOKEN_IDENTIFIER){
        ST_Item *newParamSymbol = st_insert_symbol(ptr_scanner->st_stack.top->symtable,
                                                   &token[token_index].attribute.string_val, false);

        // GENERATE
        param_number++;
        char par_num_str[10];
        sprintf(par_num_str, "LF@%%%d", param_number);

        if (in_for){
            char for_defvar_str[1000];
            sprintf(for_defvar_str, "DEFVAR LF@%s\n", token[token_index].attribute.string_val.string);
            string_add_string(&defvars_in_for, for_defvar_str);
        }
        else {
            gen_defvar_lf(token[token_index].attribute.string_val.string);
        }

        gen_move_to_lf(token[token_index].attribute.string_val.string, par_num_str);
        // GENERATE END

        int item_type;
        tmp_result = item(ptr_scanner, token, &item_type);
        if (tmp_result != SYNTAX_OK) {
            return tmp_result;
        }
        st_add_function_param(ptr_curr_symbol, item_type);
        newParamSymbol->type = item_type;
    }

    return param(ptr_scanner, token, ptr_curr_symbol, param_number);
}

int param_list(scannerT *ptr_scanner, tokenT token[], ST_Item *ptr_curr_symbol){
    if (!unget_token) {
        get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // function parameter (ID) or )
    }
    else {
        unget_token = false;
    }

    int param_number = 1;
    if (token[token_index].token_type == TOKEN_RIGHT_BRACKET){
        return SYNTAX_OK;
    }
    else if (token[token_index].token_type == TOKEN_IDENTIFIER){
        int item_type;
        Symtable *ptr_curr_scope_sym_table = stack_top(&ptr_scanner->st_stack).symtable;
        ST_Item *newParamSymbol = st_insert_symbol(ptr_curr_scope_sym_table, &token[token_index].attribute.string_val,
                                                   false);

        // GENERATE
        printf("# init parameters\n");
        if (in_for){
            char for_defvar_str[1000];
            sprintf(for_defvar_str, "DEFVAR LF@%s\n", token[token_index].attribute.string_val.string);
            string_add_string(&defvars_in_for, for_defvar_str);
        }
        else {
            gen_defvar_lf(token[token_index].attribute.string_val.string);
        }

        gen_move_to_lf(token[token_index].attribute.string_val.string, "LF@%1");
        // GENERATE END

        tmp_result = item(ptr_scanner, token, &item_type);
        if (tmp_result != SYNTAX_OK) {
            return tmp_result;
        }
        st_add_function_param(ptr_curr_symbol, item_type);
        newParamSymbol->type = item_type;
    }
    else {
        err_print("function parameter (ID) or )", token[token_index].token_type);
        return RC_SYN_ERR;
    }

    return param(ptr_scanner, token, ptr_curr_symbol, param_number);
}

int func(scannerT *ptr_scanner, tokenT token[]){
    // For different handling of main function exit
    static bool in_main = false;

    if (!unget_token) {
        get_next_token(ptr_scanner, &token[++token_index], FORBIDDEN); // function name (ID)
    }
    else {
        unget_token = false;
    }

    if (token[token_index].token_type != TOKEN_IDENTIFIER){
        err_print("identifier (function name)", token[token_index].token_type);
        return RC_SYN_ERR;
    }

    // SEMANTIC: Add function to symtable
    Symtable *ptr_curr_scope_sym_table = stack_top(&ptr_scanner->st_stack).symtable;
    ST_Item *symbol = st_insert_symbol(ptr_curr_scope_sym_table, &token[token_index].attribute.string_val, true);
    // END SEMANTIC

    // GENERATE
    if (strcmp(token[token_index].attribute.string_val.string, "main") == 0){
        in_main = true;
    }
    gen_start_func(token[token_index].attribute.string_val.string);
    indent_level = 1;
    //END GENERATE

    if (!unget_token) {
        get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // left opening bracket
    }
    else {
        unget_token = false;
    }

    if (token[token_index].token_type != TOKEN_LEFT_BRACKET){
        err_print("left opening bracket", token[token_index].token_type);
        return RC_SYN_ERR;
    }

    stack_push(&ptr_scanner->st_stack);
    tmp_result = param_list(ptr_scanner, token, symbol);
    if (tmp_result != SYNTAX_OK)
        return tmp_result;

    tmp_result = return_type_list(ptr_scanner, token, symbol);
    if (tmp_result != SYNTAX_OK)
        return tmp_result;

    printf("\n# function body\n");

    tmp_result = command_list(ptr_scanner, token);
    if (tmp_result != SYNTAX_OK)
        return tmp_result;

    if (indent_level == 0){
        if (in_main){
            gen_exit_main();
            in_main = false;
        }
        else {
            gen_popframe();
            gen_return();
        }
    }

    return SYNTAX_OK;
}

int program(scannerT *ptr_scanner, tokenT token[]){
    if (!unget_token) {
        get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // "func" / EOF
    }
    else {
        unget_token = false;
    }

    if (token[token_index].token_type == TOKEN_KEYWORD_FUNC){
        tmp_result = func(ptr_scanner, token);
        if (tmp_result != SYNTAX_OK)
            return tmp_result;

        return program(ptr_scanner, token);
    }
    else if (token[token_index].token_type == TOKEN_EOF){
        return SYNTAX_OK;
    }

    err_print("func keyword or EOL", token[token_index].token_type);
    return RC_SYN_ERR;
}

int parse(scannerT *ptr_scanner, tokenT token[]){
    late_check_stack_init(&semantic_late_check_stack);

    token_array_init(left_side_assignments, LEFT_SIDE_TOKEN_COUNT);
    string_init(&defvars_in_for);

    get_next_token(ptr_scanner, &token[token_index], OPTIONAL); // "package"

    if (token[token_index].token_type != TOKEN_KEYWORD_PACKAGE){
        err_print("package keyword", token[token_index].token_type);
        return RC_SYN_ERR;
    }

    get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // "main"
    // GENERATE
    gen_enter_main();

    if (token[token_index].token_type != TOKEN_IDENTIFIER ||
    string_compare_constant(&token[token_index].attribute.string_val, "main") != 0){
        err_print("main keyword", token[token_index].token_type);
        return RC_SYN_ERR;
    }

    tmp_result = program(ptr_scanner, token);

    if (tmp_result == SYNTAX_OK) {
        tmp_result = check_semantic_for_methods_call(&semantic_late_check_stack, &ptr_scanner->st_stack);

        if (tmp_result != SYNTAX_OK)
            return tmp_result;
        else {
            token_array_free(left_side_assignments, LEFT_SIDE_TOKEN_COUNT);

            // GENERATE
            printf("\n# defvars in cycles to avoid redefinitions\n\n%s", defvars_in_for.string);
            string_free(&defvars_in_for);
            gen_def_builtin_functions(builtin_func_used);
        }
    }
    return tmp_result;
}

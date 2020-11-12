/**
 * Project: Implementation of a compiler of the IFJ20 language.
 *
 * @file parser.c
 * @brief Parser implementation
 * @author Dominik Večeřa <xvecer23@stud.fit.vutbr.cz>
 * @date 11. 11. 2020
 */

#include "parser.h"

int token_index = 0, tmp_result = 0;
bool unget_token = false;

/*
 * Print error string to stderr
 */
void err_print(char* str, int token_type){
    fprintf(stderr, "Expected %s but got token %d.\n", str, token_type);
}

/*
 * An item - integer, float or string
 */
int item(scannerT *ptr_scanner, tokenT token[]){
    if (!unget_token) {
        get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // int, float or string literal
        printf("%d\n", token[token_index].token_type);
    }
    else {
        unget_token = false;
    }

    if (token[token_index].token_type == TOKEN_INTEGER_LITERAL ||
        token[token_index].token_type == TOKEN_DECIMAL_LITERAL ||
        token[token_index].token_type == TOKEN_STRING_LITERAL) {
        return SYNTAX_OK;
    }

    return RC_SYN_ERR;
}

/*
 * The optional continuation of a print statement
 */
int print_next(scannerT *ptr_scanner, tokenT token[]){
    get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // , or )
    printf("%d\n", token[token_index].token_type);

    switch (token[token_index].token_type) {
        case TOKEN_RIGHT_BRACKET:
            return SYNTAX_OK;

        case TOKEN_COMMA:
            get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // int / float / str, id
            printf("%d\n", token[token_index].token_type);

            if (token[token_index].token_type == TOKEN_IDENTIFIER){
                return print_next(ptr_scanner, token);
            }
            else {
                unget_token = true;
                tmp_result = item(ptr_scanner, token);
                if (tmp_result != SYNTAX_OK)
                    return tmp_result;

                return print_next(ptr_scanner, token);
            }

        default:
            err_print(", or )", token[token_index].token_type);
            return RC_SYN_ERR;
    }
}

/*
 * The beginning of a print statement
 */
int print(scannerT *ptr_scanner, tokenT token[]){
    get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // int / float / str, id, )
    printf("Print start, token: %d\n", token[token_index].token_type);

    switch (token[token_index].token_type) {
        case TOKEN_RIGHT_BRACKET:
            return SYNTAX_OK;

        case TOKEN_IDENTIFIER:
            return print_next(ptr_scanner, token);

        default:
            unget_token = true;
            tmp_result = item(ptr_scanner, token);
            if (tmp_result != SYNTAX_OK)
                return tmp_result;

            return print_next(ptr_scanner, token);
    }
}

/*
 * A built-in function
 */
int builtin_func(scannerT *ptr_scanner, tokenT token[]){
    if (!unget_token) {
        get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // built-in function name
        printf("%d\n", token[token_index].token_type);
    }
    else {
        unget_token = false;
    }

    switch (token[token_index].token_type) {
        case TOKEN_FUNCTION_INPUTS:
        case TOKEN_FUNCTION_INPUTI:
        case TOKEN_FUNCTION_INPUTF:
            get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // (
            printf("%d\n", token[token_index].token_type);

            if (token[token_index].token_type != TOKEN_LEFT_BRACKET){
                err_print("(", token[token_index].token_type);
                return RC_SYN_ERR;
            }

            get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // )
            printf("%d\n", token[token_index].token_type);

            if (token[token_index].token_type == TOKEN_RIGHT_BRACKET){
                return SYNTAX_OK;
            }
            else {
                err_print(")", token[token_index].token_type);
                return RC_SYN_ERR;
            }

        case TOKEN_FUNCTION_INT2FLOAT:
        case TOKEN_FUNCTION_CHR:
            get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // (
            printf("%d\n", token[token_index].token_type);

            if (token[token_index].token_type != TOKEN_LEFT_BRACKET){
                err_print("(", token[token_index].token_type);
                return RC_SYN_ERR;
            }

            get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // int
            printf("%d\n", token[token_index].token_type);

            if (token[token_index].token_type != TOKEN_INTEGER_LITERAL){
                err_print("int", token[token_index].token_type);
                return RC_SYN_ERR;
            }

            get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // )
            printf("%d\n", token[token_index].token_type);

            if (token[token_index].token_type == TOKEN_RIGHT_BRACKET){
                return SYNTAX_OK;
            }
            else {
                err_print(")", token[token_index].token_type);
                return RC_SYN_ERR;
            }

        case TOKEN_FUNCTION_PRINT:
            get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // (
            printf("%d\n", token[token_index].token_type);

            if (token[token_index].token_type != TOKEN_LEFT_BRACKET){
                err_print("(", token[token_index].token_type);
                return RC_SYN_ERR;
            }

            return print(ptr_scanner, token);

        case TOKEN_FUNCTION_FLOAT2INT:
            get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // (
            printf("%d\n", token[token_index].token_type);

            if (token[token_index].token_type != TOKEN_LEFT_BRACKET){
                err_print("(", token[token_index].token_type);
                return RC_SYN_ERR;
            }

            get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // float
            printf("%d\n", token[token_index].token_type);

            if (token[token_index].token_type != TOKEN_KEYWORD_FLOAT64){
                err_print("float", token[token_index].token_type);
                return RC_SYN_ERR;
            }

            get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // )
            printf("%d\n", token[token_index].token_type);

            if (token[token_index].token_type == TOKEN_RIGHT_BRACKET){
                return SYNTAX_OK;
            }
            else {
                err_print(")", token[token_index].token_type);
                return RC_SYN_ERR;
            }

        case TOKEN_FUNCTION_LEN:
            get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // (
            printf("%d\n", token[token_index].token_type);

            if (token[token_index].token_type != TOKEN_LEFT_BRACKET){
                err_print("(", token[token_index].token_type);
                return RC_SYN_ERR;
            }

            get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // string
            printf("%d\n", token[token_index].token_type);

            if (token[token_index].token_type != TOKEN_STRING_LITERAL){
                err_print("string", token[token_index].token_type);
                return RC_SYN_ERR;
            }

            get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // )
            printf("%d\n", token[token_index].token_type);

            if (token[token_index].token_type == TOKEN_RIGHT_BRACKET){
                return SYNTAX_OK;
            }
            else {
                err_print(")", token[token_index].token_type);
                return RC_SYN_ERR;
            }

        case TOKEN_FUNCTION_SUBSTR:
            get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // (
            printf("%d\n", token[token_index].token_type);

            if (token[token_index].token_type != TOKEN_LEFT_BRACKET){
                err_print("(", token[token_index].token_type);
                return RC_SYN_ERR;
            }

            get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // string
            printf("%d\n", token[token_index].token_type);

            if (token[token_index].token_type != TOKEN_STRING_LITERAL){
                err_print("string", token[token_index].token_type);
                return RC_SYN_ERR;
            }

            get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // ,
            printf("%d\n", token[token_index].token_type);

            if (token[token_index].token_type != TOKEN_COMMA){
                err_print(",", token[token_index].token_type);
                return RC_SYN_ERR;
            }

            get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // int
            printf("%d\n", token[token_index].token_type);

            if (token[token_index].token_type != TOKEN_INTEGER_LITERAL){
                err_print("int", token[token_index].token_type);
                return RC_SYN_ERR;
            }

            get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // ,
            printf("%d\n", token[token_index].token_type);

            if (token[token_index].token_type != TOKEN_COMMA){
                err_print(",", token[token_index].token_type);
                return RC_SYN_ERR;
            }

            get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // int
            printf("%d\n", token[token_index].token_type);

            if (token[token_index].token_type != TOKEN_INTEGER_LITERAL){
                err_print("int", token[token_index].token_type);
                return RC_SYN_ERR;
            }

            get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // )
            printf("%d\n", token[token_index].token_type);

            if (token[token_index].token_type == TOKEN_RIGHT_BRACKET){
                return SYNTAX_OK;
            }
            else {
                err_print(")", token[token_index].token_type);
                return RC_SYN_ERR;
            }

        case TOKEN_FUNCTION_ORD:
            get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // (
            printf("%d\n", token[token_index].token_type);

            if (token[token_index].token_type != TOKEN_LEFT_BRACKET){
                err_print("(", token[token_index].token_type);
                return RC_SYN_ERR;
            }

            get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // string
            printf("%d\n", token[token_index].token_type);

            if (token[token_index].token_type != TOKEN_STRING_LITERAL){
                err_print("string", token[token_index].token_type);
                return RC_SYN_ERR;
            }

            get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // ,
            printf("%d\n", token[token_index].token_type);

            if (token[token_index].token_type != TOKEN_COMMA){
                err_print(",", token[token_index].token_type);
                return RC_SYN_ERR;
            }

            get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // int
            printf("%d\n", token[token_index].token_type);

            if (token[token_index].token_type != TOKEN_INTEGER_LITERAL){
                err_print("int", token[token_index].token_type);
                return RC_SYN_ERR;
            }

            get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // )
            printf("%d\n", token[token_index].token_type);

            if (token[token_index].token_type == TOKEN_RIGHT_BRACKET){
                return SYNTAX_OK;
            }
            else {
                err_print(")", token[token_index].token_type);
                return RC_SYN_ERR;
            }

        default:
            return RC_SYN_ERR;
    }
}

/*
 * An expression - resolved by precedential syntactic analysis
 */
int expr(scannerT *ptr_scanner, tokenT token[]){
    if (!unget_token) {
        get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // beginning of expr
        printf("%d\n", token[token_index].token_type);
    }
    else {
        unget_token = false;
    }

    // TODO according to existing implementation of PSA
    return SYNTAX_OK;
}

/*
 * An individual assignment
 */
int assign(scannerT *ptr_scanner, tokenT token[]){
    if (!unget_token) {
        get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // id, item, built-in function or expression
        printf("%d\n", token[token_index].token_type);
    }
    else {
        unget_token = false;
    }

    if (token[token_index].token_type == TOKEN_IDENTIFIER){
        // check following token, if (, then function parameter list follows, otherwise an extra token was read
        get_next_token(ptr_scanner, &token[++token_index], OPTIONAL);
        printf("%d\n", token[token_index].token_type);

        if (token[token_index].token_type == TOKEN_LEFT_BRACKET) {
            return id_list1(ptr_scanner, token);
        }
        else {
            unget_token = true;
            return SYNTAX_OK;
        }
    }

    unget_token = true;
    tmp_result = item(ptr_scanner, token);
    if (tmp_result == SYNTAX_OK)
        return SYNTAX_OK;

    unget_token = true;
    tmp_result = builtin_func(ptr_scanner, token);
    if (tmp_result == SYNTAX_OK)
        return SYNTAX_OK;

    return expr(ptr_scanner, token); // TODO expr?
}

/*
 * The optional continuation of a list of assignments
 */
int assign_next(scannerT *ptr_scanner, tokenT token[]){
    if (!unget_token) {
        get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // , or extra token was read
        printf("%d\n", token[token_index].token_type);
    }
    else {
        unget_token = false;
    }

    if (token[token_index].token_type == TOKEN_COMMA){
        tmp_result = assign(ptr_scanner, token);
        if (tmp_result != SYNTAX_OK)
            return tmp_result;

        return assign_next(ptr_scanner, token);
    }
    else {
        unget_token = true;
        return SYNTAX_OK;
    }
}

/*
 * The beginning of a list of assignments
 */
int assign_list(scannerT *ptr_scanner, tokenT token[]){
    if (!unget_token) {
        get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // id, item, built-in function or expression
        printf("%d\n", token[token_index].token_type);
    }
    else {
        unget_token = false;
    }

    unget_token = true;
    tmp_result = assign(ptr_scanner, token);
    if (tmp_result != SYNTAX_OK)
        return tmp_result;

    return assign_next(ptr_scanner, token);
}

/*
 * The optional continuation of a list of identifiers
 */
int id_next2(scannerT *ptr_scanner, tokenT token[]){
    get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // = or ,
    printf("%d\n", token[token_index].token_type);

    switch (token[token_index].token_type){
        case TOKEN_EQUAL:
            return SYNTAX_OK;

        case TOKEN_COMMA:
            get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // id
            printf("%d\n", token[token_index].token_type);

            if (token[token_index].token_type != TOKEN_IDENTIFIER){
                err_print("id", token[token_index].token_type);
                return RC_SYN_ERR;
            }

            return id_next2(ptr_scanner, token);

        default:
            err_print("= or ,", token[token_index].token_type);
            return RC_SYN_ERR;
    }
}

/*
 * The beginning of a list of identifiers
 */
int id_list2(scannerT *ptr_scanner, tokenT token[]){
    get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // id or =
    printf("%d\n", token[token_index].token_type);

    switch (token[token_index].token_type){
        case TOKEN_EQUAL:
            return SYNTAX_OK;

        case TOKEN_IDENTIFIER:
            return id_next2(ptr_scanner, token);

        default:
            err_print("id or =", token[token_index].token_type);
            return RC_SYN_ERR;
    }
}

/*
 * The optional continuation of a list of identifiers
 */
int id_next1(scannerT *ptr_scanner, tokenT token[]){
    get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // ) or ,
    printf("%d\n", token[token_index].token_type);

    switch (token[token_index].token_type){
        case TOKEN_RIGHT_BRACKET:
            return SYNTAX_OK;

        case TOKEN_COMMA:
            get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); //  id
            printf("%d\n", token[token_index].token_type);

            if (token[token_index].token_type != TOKEN_IDENTIFIER){
                err_print("id", token[token_index].token_type);
                return RC_SYN_ERR;
            }

            return id_next1(ptr_scanner, token);

        default:
            err_print(") or ,", token[token_index].token_type);
            return RC_SYN_ERR;
    }
}

/*
 * The beginning of a list of identifiers
 */
int id_list1(scannerT *ptr_scanner, tokenT token[]){
    get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // id or )
    printf("%d\n", token[token_index].token_type);

    switch (token[token_index].token_type){
        case TOKEN_RIGHT_BRACKET:
            return SYNTAX_OK;

        case TOKEN_IDENTIFIER:
            return id_next1(ptr_scanner, token);

        default:
            err_print("id or )", token[token_index].token_type);
            return RC_SYN_ERR;
    }
}

/*
 * A command with an identifier (definition, one or multiple assignments, function call)
 */
int id_command(scannerT *ptr_scanner, tokenT token[]){
    get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // :=, =, ( or ,
    printf("%d\n", token[token_index].token_type);

    switch (token[token_index].token_type){
        case TOKEN_COLON_EQUAL:
        case TOKEN_EQUAL:
            return assign(ptr_scanner, token);

        case TOKEN_LEFT_BRACKET:
            return id_list1(ptr_scanner, token);

        case TOKEN_COMMA:
            tmp_result = id_list2(ptr_scanner, token);
            if (tmp_result != SYNTAX_OK)
                return tmp_result;

            return assign_list(ptr_scanner, token);

        default:
            err_print(":=, =, ( or ,", token[token_index].token_type);
            return RC_SYN_ERR;
    }
}

/*
 * An optional assignment at the end of each cycle iteration
 */
int cycle_assign(scannerT *ptr_scanner, tokenT token[]){
    get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // id or {
    printf("%d\n", token[token_index].token_type);

    if (token[token_index].token_type == TOKEN_LEFT_CURLY_BRACE){
        return SYNTAX_OK;
    }
    else if (token[token_index].token_type == TOKEN_IDENTIFIER){
        get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // =
        printf("%d\n", token[token_index].token_type);

        if (token[token_index].token_type != TOKEN_EQUAL){
            err_print("=", token[token_index].token_type);
            return RC_SYN_ERR;
        }

        tmp_result = expr(ptr_scanner, token); // TODO expr?
        if (tmp_result != SYNTAX_OK)
            return tmp_result;

        if (!unget_token) {
            get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // {
            printf("%d\n", token[token_index].token_type);
        }
        else {
            unget_token = false;
        }

        if (token[token_index].token_type != TOKEN_LEFT_CURLY_BRACE){
            err_print("{", token[token_index].token_type);
            return RC_SYN_ERR;
        }

        return SYNTAX_OK;
    }
    else {
        err_print("id or {", token[token_index].token_type);
        return RC_SYN_ERR;
    }
}

/*
 * Optional initial definition in a cycle
 */
int cycle_init(scannerT *ptr_scanner, tokenT token[]){
    get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // id or ;
    printf("%d\n", token[token_index].token_type);

    if (token[token_index].token_type == TOKEN_SEMICOLON){
        return SYNTAX_OK;
    }
    else if (token[token_index].token_type == TOKEN_IDENTIFIER){
        get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // :=
        printf("%d\n", token[token_index].token_type);

        if (token[token_index].token_type != TOKEN_COLON_EQUAL){
            err_print(":=", token[token_index].token_type);
            return RC_SYN_ERR;
        }

        tmp_result = assign(ptr_scanner, token);
        if (tmp_result != SYNTAX_OK)
            return tmp_result;

        if (!unget_token) {
            get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // ;
            printf("%d\n", token[token_index].token_type);
        }
        else {
            unget_token = false;
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

/*
 * An individual command inside a function, cycle, or block
 */
int command(scannerT *ptr_scanner, tokenT token[]){
    // probably shouldn't get token here, since parser already has it from cmd_list

    switch (token[token_index].token_type){
        case TOKEN_IDENTIFIER:
            return id_command(ptr_scanner, token);

        case TOKEN_KEYWORD_IF: // TODO expr?
            tmp_result = expr(ptr_scanner, token);
            if (tmp_result != SYNTAX_OK)
                return tmp_result;

            if (!unget_token) {
                get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // {
                printf("%d\n", token[token_index].token_type);
            }
            else {
                unget_token = false;
            }

            if (token[token_index].token_type != TOKEN_LEFT_CURLY_BRACE){
                err_print("{", token[token_index].token_type);
                return RC_SYN_ERR;
            }

            tmp_result = command_list(ptr_scanner, token);
            if (tmp_result != SYNTAX_OK)
                return tmp_result;

            if (!unget_token) {
                get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // else
                printf("%d\n", token[token_index].token_type);
            }
            else {
                unget_token = false;
            }

            if (token[token_index].token_type != TOKEN_KEYWORD_ELSE){
                err_print("else keyword", token[token_index].token_type);
                return RC_SYN_ERR;
            }

            get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // {
            printf("%d\n", token[token_index].token_type);

            if (token[token_index].token_type != TOKEN_LEFT_CURLY_BRACE){
                err_print("{", token[token_index].token_type);
                return RC_SYN_ERR;
            }

            return command_list(ptr_scanner, token);

        case TOKEN_KEYWORD_FOR:
            tmp_result = cycle_init(ptr_scanner, token);
            if (tmp_result != SYNTAX_OK)
                return tmp_result;

            tmp_result = expr(ptr_scanner, token); // TODO expr?
            if (tmp_result != SYNTAX_OK)
                return tmp_result;

            if (!unget_token) {
                get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // ;
                printf("%d\n", token[token_index].token_type);
            }
            else {
                unget_token = false;
            }

            if (token[token_index].token_type != TOKEN_SEMICOLON){
                err_print(";", token[token_index].token_type);
                return RC_SYN_ERR;
            }

            tmp_result = cycle_assign(ptr_scanner, token);
            if (tmp_result != SYNTAX_OK)
                return tmp_result;

            return command_list(ptr_scanner, token);

        case TOKEN_KEYWORD_RETURN:
            // a function can return zero or more values
            get_next_token(ptr_scanner, &token[++token_index], OPTIONAL);  // check }
            printf("%d\n", token[token_index].token_type);

            unget_token = true;
            if (token[token_index].token_type == TOKEN_RIGHT_CURLY_BRACE){
                return SYNTAX_OK;
            }
            else {
                return assign_list(ptr_scanner, token);
            }

        default:
            break;
    }

    unget_token = true;
    tmp_result = builtin_func(ptr_scanner, token);
    if (tmp_result == SYNTAX_OK)
        return SYNTAX_OK;

    return expr(ptr_scanner, token); // TODO expr?
}

/*
 * The list of commands inside a function, cycle, or block
 */
int command_list(scannerT *ptr_scanner, tokenT token[]){
    if (!unget_token) {
        get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // command or }
        printf("%d\n", token[token_index].token_type);
    }
    else {
        unget_token = false;
    }

    if (token[token_index].token_type == TOKEN_RIGHT_CURLY_BRACE){
        return SYNTAX_OK;
    }
    else {
        tmp_result = command(ptr_scanner, token);
        if (tmp_result != SYNTAX_OK)
            return tmp_result;

        return command_list(ptr_scanner, token);
    }
}

/*
 * The optional continuation of a list of function return types
 */
int return_type(scannerT *ptr_scanner, tokenT token[]){
    get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // , or )
    printf("%d\n", token[token_index].token_type);

    if (token[token_index].token_type == TOKEN_RIGHT_BRACKET){
        get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // {
        printf("%d\n", token[token_index].token_type);

        if (token[token_index].token_type == TOKEN_LEFT_CURLY_BRACE){
            return SYNTAX_OK;
        }
        else {
            err_print("{", token[token_index].token_type);
            return RC_SYN_ERR;
        }
    }
    else if (token[token_index].token_type == TOKEN_COMMA){
        tmp_result = item(ptr_scanner, token);
        if (tmp_result != SYNTAX_OK)
            return tmp_result;

        return return_type(ptr_scanner, token);
    }
    else {
        err_print(", or }", token[token_index].token_type);
        return RC_SYN_ERR;
    }
}

/*
 * The beginning of a list of function return types
 */
int return_type_list(scannerT *ptr_scanner, tokenT token[]){
    get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // ( or {
    printf("%d\n", token[token_index].token_type);

    puts("-----------------------------------");
    for (int i = 0; i < TOKEN_ARRAY_LEN; i++)
        printf("%d: %d\n", i, token[i].token_type);
    puts("-----------------------------------");

    if (token[token_index].token_type == TOKEN_LEFT_CURLY_BRACE){
        return SYNTAX_OK;
    }
    else if (token[token_index].token_type != TOKEN_LEFT_BRACKET){
        err_print("( or {", token[token_index].token_type);
        return RC_SYN_ERR;
    }

    get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // ) or item
    printf("%d\n", token[token_index].token_type);

    if (token[token_index].token_type == TOKEN_RIGHT_BRACKET){
        get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // {
        printf("%d\n", token[token_index].token_type);

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
        tmp_result = item(ptr_scanner, token);
        if (tmp_result != SYNTAX_OK)
            return tmp_result;

        return return_type(ptr_scanner, token);
    }
}

/*
 * The optional continuation of a list of function parameters
 */
int param(scannerT *ptr_scanner, tokenT token[]){
    get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // , or )
    printf("%d\n", token[token_index].token_type);

    if (token[token_index].token_type == TOKEN_RIGHT_BRACKET){
        return SYNTAX_OK;
    }
    else if (token[token_index].token_type != TOKEN_COMMA){
        err_print(", or )", token[token_index].token_type);
        return RC_SYN_ERR;
    }

    get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // function parameter (ID)
    printf("%d\n", token[token_index].token_type);

    if (token[token_index].token_type == TOKEN_IDENTIFIER){
        tmp_result = item(ptr_scanner, token);
        if (tmp_result != SYNTAX_OK)
            return tmp_result;
    }

    return param(ptr_scanner, token);
}

/*
 * The beginning of a list of function parameters
 */
int param_list(scannerT *ptr_scanner, tokenT token[]){
    get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // function parameter (ID) or )
    printf("%d\n", token[token_index].token_type);

    if (token[token_index].token_type == TOKEN_RIGHT_BRACKET){
        return SYNTAX_OK;
    }
    else if (token[token_index].token_type == TOKEN_IDENTIFIER){
        tmp_result = item(ptr_scanner, token);
        if (tmp_result != SYNTAX_OK)
            return tmp_result;
    }
    else {
        err_print("function parameter (ID) or )", token[token_index].token_type);
        return RC_SYN_ERR;
    }

    return param(ptr_scanner, token);
}

/*
 * A function definition
 */
int func(scannerT *ptr_scanner, tokenT token[]){
    get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // function name (ID)
    printf("%d\n", token[token_index].token_type);

    if (token[token_index].token_type != TOKEN_IDENTIFIER){
        err_print("identifier (function name)", token[token_index].token_type);
        return RC_SYN_ERR;
    }

    get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // left opening bracket
    printf("%d\n", token[token_index].token_type);

    if (token[token_index].token_type != TOKEN_LEFT_BRACKET){
        err_print("left opening bracket", token[token_index].token_type);
        return RC_SYN_ERR;
    }

    tmp_result = param_list(ptr_scanner, token);
    if (tmp_result != SYNTAX_OK)
        return tmp_result;

    tmp_result = return_type_list(ptr_scanner, token);
    if (tmp_result != SYNTAX_OK)
        return tmp_result;

    tmp_result = command_list(ptr_scanner, token);
    if (tmp_result != SYNTAX_OK)
        return tmp_result;

    return SYNTAX_OK;
}

/*
 * The program itself - a list of functions
 */
int program(scannerT *ptr_scanner, tokenT token[]){
    get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // "func" / EOF
    printf("%d\n", token[token_index].token_type);

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

/*
 * Starting point of parser
 */
int parse(scannerT *ptr_scanner, tokenT token[]){
    get_next_token(ptr_scanner, &token[token_index], OPTIONAL); // "package"
    printf("%d\n", token[token_index].token_type);

    if (token[token_index].token_type != TOKEN_KEYWORD_PACKAGE){
        err_print("package keyword", token[token_index].token_type);
        return RC_SYN_ERR;
    }

    get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // "main"
    printf("%d\n", token[token_index].token_type);

    if (token[token_index].token_type != TOKEN_IDENTIFIER &&
    !string_compare_constant(&token[token_index].attribute.string_val, "main\n")){ // TODO remove newline
        err_print("main keyword", token[token_index].token_type);
        return RC_SYN_ERR;
    }

    return program(ptr_scanner, token);
}

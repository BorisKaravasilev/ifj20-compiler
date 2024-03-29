/**
 * Project: Implementation of a compiler of the IFJ20 language.
 * @file    token_functions.c
 * @brief   This file contains the implementation of the token functions.
 * @author  Petr Vrtal <xvrtal01@stud.fit.vutbr.cz>
 * @author  Robin Stancl <xstanc09@stud.fit.vutbr.cz>
 * @date    13. 10. 2020
 */

#include <stdlib.h>
#include <stdio.h>
#include "token_functions.h"
#include "token_types.h"
#include "debugging.h"

// Initializes token structure
void token_init (tokenT *ptr_token) {
    ptr_token->token_type = TOKEN_EMPTY;
    ptr_token->attribute.symtable_item = NULL;
    string_init(&ptr_token->attribute.string_val);
}

// Initializes array of tokens
void token_array_init(tokenT *ptr_token_array, int array_length) {
    for (int i = 0; i < array_length; i++) {
        token_init(&ptr_token_array[i]);
    }
}

// Adds a character to token's attribute string value
void token_val_add_char(tokenT *ptr_token, char ch) {
    string_add_character(&ptr_token->attribute.string_val, ch);
}

// Sets token structure members to default values
void token_clear(tokenT *ptr_token) {
    if (ptr_token == NULL) {
        return;
    }

    ptr_token->token_type = TOKEN_EMPTY;

    if (ptr_token->attribute.symtable_item != NULL) {
        free(ptr_token->attribute.symtable_item);
    }

    ptr_token->attribute.symtable_item = NULL;
    string_clear(&ptr_token->attribute.string_val);
}

// Dealocates memory of token structure and it's members
void token_free(tokenT *ptr_token) {
    if (ptr_token == NULL)
        return;

    clear_str(&ptr_token->attribute.string_val);
}

// Dealocates memory of the token array
void token_array_free(tokenT *ptr_token_array, int array_length) {
    for (int i = 0; i < array_length; i++) {
        token_free(&ptr_token_array[i]);
    }
}

// Checks if a token is keyword, if yes, returns the correct token type
int keyword_check(tokenT *ptr_token) {
    int original_type = ptr_token->token_type;

    if (string_compare_constant(&ptr_token->attribute.string_val, "else") == 0) {
        return TOKEN_KEYWORD_ELSE;
    }
    else if (string_compare_constant(&ptr_token->attribute.string_val, "float64") == 0) {
        return TOKEN_KEYWORD_FLOAT64;
    }
    else if (string_compare_constant(&ptr_token->attribute.string_val, "for") == 0) {
        return TOKEN_KEYWORD_FOR;
    }
    else if (string_compare_constant(&ptr_token->attribute.string_val, "func") == 0) {
        return TOKEN_KEYWORD_FUNC;
    }
    else if (string_compare_constant(&ptr_token->attribute.string_val, "if") == 0) {
        return TOKEN_KEYWORD_IF;
    }
    else if (string_compare_constant(&ptr_token->attribute.string_val, "int") == 0) {
        return TOKEN_KEYWORD_INT;
    }
    else if (string_compare_constant(&ptr_token->attribute.string_val, "package") == 0) {
        return TOKEN_KEYWORD_PACKAGE;
    }
    else if (string_compare_constant(&ptr_token->attribute.string_val, "return") == 0) {
        return TOKEN_KEYWORD_RETURN;
    }
    else if (string_compare_constant(&ptr_token->attribute.string_val, "string") == 0) {
        return TOKEN_KEYWORD_STRING;
    }

    return original_type;
}

// Checks if a token is a built-in function name, if yes, returns the correct token type
int function_word_check(tokenT *ptr_token) {
    int original_type = ptr_token->token_type;

    if (string_compare_constant(&ptr_token->attribute.string_val, "inputs") == 0) {
        return TOKEN_FUNCTION_INPUTS;
    }
    else if (string_compare_constant(&ptr_token->attribute.string_val, "inputi") == 0) {
        return TOKEN_FUNCTION_INPUTI;
    }
    else if (string_compare_constant(&ptr_token->attribute.string_val, "inputf") == 0) {
        return TOKEN_FUNCTION_INPUTF;
    }
    else if (string_compare_constant(&ptr_token->attribute.string_val, "print") == 0) {
        return TOKEN_FUNCTION_PRINT;
    }
    else if (string_compare_constant(&ptr_token->attribute.string_val, "len") == 0) {
        return TOKEN_FUNCTION_LEN;
    }
    else if (string_compare_constant(&ptr_token->attribute.string_val, "substr") == 0) {
        return TOKEN_FUNCTION_SUBSTR;
    }
    else if (string_compare_constant(&ptr_token->attribute.string_val, "ord") == 0) {
        return TOKEN_FUNCTION_ORD;
    }
    else if (string_compare_constant(&ptr_token->attribute.string_val, "chr") == 0) {
        return TOKEN_FUNCTION_CHR;
    }
    else if (string_compare_constant(&ptr_token->attribute.string_val, "int2float") == 0) {
        return TOKEN_FUNCTION_INT2FLOAT;
    }
    else if (string_compare_constant(&ptr_token->attribute.string_val, "float2int") == 0) {
        return TOKEN_FUNCTION_FLOAT2INT;
    }
    return original_type;
}

// Prints debugging information about a token (if debugging macros are set to debugging mode in debugging.h)
void debug_token(tokenT *ptr_token, int token_index) {
    debug_scanner("[ %d --> Received token type: '%d', ", token_index, ptr_token->token_type);
    debug_scanner("attribute: '%s']\n", ptr_token->attribute.string_val.string);
}

// Copies the source token "t_src" into the destination token "t_dst"
void copy_token(tokenT *t_dst, tokenT *t_src) {
    t_dst->token_type = t_src->token_type;
    string_init(&t_dst->attribute.string_val);
    string_copy(&t_dst->attribute.string_val, &t_src->attribute.string_val);
    t_dst->attribute.symtable_item = t_src->attribute.symtable_item;
}

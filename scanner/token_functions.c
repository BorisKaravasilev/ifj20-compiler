//
// Created by Petr Vrtal on 13.10.2020.
//

#include <stdlib.h>
#include <stdio.h>
#include "token_functions.h"
#include "token_types.h"

void token_init (tokenT *ptr_token) {
    ptr_token->token_type = TOKEN_EMPTY;
    ptr_token->attribute.symtable_item = NULL;
    string_init(&ptr_token->attribute.string_val);
}

void token_val_add_char(tokenT *ptr_token, char ch) {
    string_add_character(&ptr_token->attribute.string_val, ch);
}

void token_clear(tokenT *ptr_token) { // TODO: Remove
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

void token_free(tokenT *ptr_token) {
    if (ptr_token == NULL)
        return;

    clear_str(&ptr_token->attribute.string_val);
    free(ptr_token);
}

int keyword_check(tokenT *ptr_token, int original_type) {
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

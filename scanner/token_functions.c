//
// Created by Petr Vrtal on 13.10.2020.
//

#include <stdlib.h>
#include <stdio.h>
#include "token_functions.h"
#include "token_types.h"

void token_init (tokenT *token) {
    token->token_type = TOKEN_EMPTY;
    token->attribute.symtable_item = NULL;
    string_init(&token->attribute.string_val);
}

void token_val_add_char(tokenT *token, char ch) {
    string_add_character(&token->attribute.string_val, ch);
}

void token_clear(tokenT *token) { // TODO: Remove
    if (token == NULL) {
        return;
    }

    token->token_type = TOKEN_EMPTY;

    if (token->attribute.symtable_item != NULL) {
        free(token->attribute.symtable_item);
    }

    token->attribute.symtable_item = NULL;
    string_clear(&token->attribute.string_val);
}

void token_free(tokenT *token) {
    if (token == NULL)
        return;

    clear_str(&token->attribute.string_val);
    free(token);
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

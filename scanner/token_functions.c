//
// Created by Petr Vrtal on 13.10.2020.
//

#include <stdlib.h>
#include <stdio.h>
#include "token_functions.h"
#include "token_types.h"

void token_init (token_struct *token) {
    string_init(&token->token_val);
}

void token_val_add_char(token_struct *token, char ch) {
    string_add_character(&token->token_val, ch);
}

void token_clear(token_struct *token) {
    if (token == NULL) {
        return;
    }

    string_clear(&token->token_val);
    token->token_type = TOKEN_EMPTY;
}

void token_free(token_struct *token) {
    if (token == NULL)
        return;

    clear_str(&token->token_val);
    free(token);
}

int keyword_check(token_struct *token, int original_type) {
    if (string_compare_constant(&token->token_val, "else") == 0) {
        return TOKEN_KEYWORD_ELSE;
    }
    else if (string_compare_constant(&token->token_val, "float64") == 0) {
        return TOKEN_KEYWORD_FLOAT64;
    }
    else if (string_compare_constant(&token->token_val, "for") == 0) {
        return TOKEN_KEYWORD_FOR;
    }
    else if (string_compare_constant(&token->token_val, "func") == 0) {
        return TOKEN_KEYWORD_FUNC;
    }
    else if (string_compare_constant(&token->token_val, "if") == 0) {
        return TOKEN_KEYWORD_IF;
    }
    else if (string_compare_constant(&token->token_val, "int") == 0) {
        return TOKEN_KEYWORD_INT;
    }
    else if (string_compare_constant(&token->token_val, "package") == 0) {
        return TOKEN_KEYWORD_PACKAGE;
    }
    else if (string_compare_constant(&token->token_val, "return") == 0) {
        return TOKEN_KEYWORD_RETURN;
    }
    else if (string_compare_constant(&token->token_val, "string") == 0) {
        return TOKEN_KEYWORD_STRING;
    }

    return original_type;
}

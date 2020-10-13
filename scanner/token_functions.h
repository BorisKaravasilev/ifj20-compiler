//
// Created by Petr Vrtal on 13.10.2020.
//

#ifndef IFJ20_COMPILER_TOKEN_FUNCTIONS_H
#define IFJ20_COMPILER_TOKEN_FUNCTIONS_H

#include "../general/string_functions.h"

typedef struct tokenT {
    int token_type;
    string token_val;
} token_struct;

void token_init (token_struct *token);
void token_val_add_char(token_struct *token, char ch);
void token_clear (token_struct *token);
void token_free (token_struct *token);

int keyword_check (token_struct *token, int original_type);

#endif //IFJ20_COMPILER_TOKEN_FUNCTIONS_H

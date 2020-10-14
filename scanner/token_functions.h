//
// Created by Petr Vrtal on 13.10.2020.
//

#ifndef LEX_TOKEN_FUNCTIONS_H
#define LEX_TOKEN_FUNCTIONS_H

#include "../general/string_functions.h"

// TODO: Create token attribute structure

typedef struct {
    // symtable_item
    char *string_val;
} attributeT;

typedef struct {
    int token_type;
    attributeT attribute;
    string token_val;
} tokenT;

void token_init (tokenT *token);
void token_val_add_char(tokenT *token, char ch);
void token_clear (tokenT *token);
void token_free (tokenT *token);

int keyword_check (tokenT *token, int original_type);

#endif //IFJ20_COMPILER_TOKEN_FUNCTIONS_H

//
// Created by Petr Vrtal on 13.10.2020.
//

#ifndef LEX_TOKEN_FUNCTIONS_H
#define LEX_TOKEN_FUNCTIONS_H

#include "../general/string_functions.h"
#include "symtable.h"

typedef struct {
    symtable_itemT *symtable_item;
    stringT string_val;
} attributeT;

typedef struct {
    int token_type;
    attributeT attribute;
} tokenT;

void token_init (tokenT *ptr_token);
void token_val_add_char(tokenT *ptr_token, char ch);
void token_clear (tokenT *ptr_token);
void token_free (tokenT *ptr_token);

int keyword_check (tokenT *ptr_token, int original_type);

#endif //IFJ20_COMPILER_TOKEN_FUNCTIONS_H

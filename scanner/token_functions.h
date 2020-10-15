//
// Created by Petr Vrtal on 13.10.2020.
//

#ifndef LEX_TOKEN_FUNCTIONS_H
#define LEX_TOKEN_FUNCTIONS_H

#define TOKEN_ARRAY_LEN 50

#include "../general/string_functions.h"
#include "symtable.h"

// TODO: Consider union
typedef struct {
    symtable_itemT *symtable_item;
    stringT string_val;
} attributeT;

typedef struct {
    int token_type;
    attributeT attribute;
} tokenT;

void token_init (tokenT *ptr_token);
void token_array_init(tokenT *ptr_token_array, int array_length);
void token_val_add_char(tokenT *ptr_token, char ch);
void token_clear (tokenT *ptr_token);
void token_free (tokenT *ptr_token);
void debug_token(tokenT *ptr_token, int token_index);

int keyword_check (tokenT *ptr_token, int original_type);

#endif //IFJ20_COMPILER_TOKEN_FUNCTIONS_H

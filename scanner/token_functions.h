/**
 * Project: Implementation of a compiler of the IFJ20 language.
 * @file    token_functions.h
 * @brief   This is the header file for token_functions.c
 * @author  Petr Vrtal <xvrtal01@stud.fit.vutbr.cz>
 * @author  Robin Stancl <xstanc09@stud.fit.vutbr.cz>
 * @date    13. 10. 2020
 */

#ifndef LEX_TOKEN_FUNCTIONS_H
#define LEX_TOKEN_FUNCTIONS_H

#define TOKEN_ARRAY_LEN 5000

#include "string_functions.h"
#include "symtable.h"

typedef struct {
    ST_Item *symtable_item;
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

int keyword_check(tokenT *ptr_token);
int function_word_check (tokenT *ptr_token);
void copy_token(tokenT *t_dst, tokenT *t_src);

#endif //IFJ20_COMPILER_TOKEN_FUNCTIONS_H

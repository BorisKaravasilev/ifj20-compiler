/**
 * Project: Implementation of a compiler of the IFJ20 language.
 * @brief   This is a header file for token_structure.c file. It includes heads of functions and definitions of constants.
 * @author  Robin Stancl <xstanc09@stud.fit.vutbr.cz>
 * @author Boris Karavasilev <xkarav01@stud.fit.vutbr.cz>
 */

#ifndef IFJ_PROJEKT_TOKEN_STRUCTURE_H
#define IFJ_PROJEKT_TOKEN_STRUCTURE_H

#include <stdbool.h>
#include "string_functions.h"
#include "token_functions.h"

/**
 * Structures
 */

/// Structure list item
typedef struct token_list_itemT
{
    tokenT token;               // Token itself
    struct token_list_itemT *next;     // Pointer to the next item
} token_list_itemT;

/// Structure list of tokens
typedef struct token_listT
{
    struct token_list_itemT *first;    // Pointer to the first item in the list
} token_listT;

// Function prototypes
void token_list_initialize(token_listT *list_of_tokens);
token_list_itemT *token_list_item_init(tokenT *token);
bool token_list_add_item(token_listT *list_of_tokens, tokenT *token_to_add);
void token_list_free(token_listT *list_of_tokens);

#endif //IFJ_PROJEKT_TOKEN_STRUCTURE_H
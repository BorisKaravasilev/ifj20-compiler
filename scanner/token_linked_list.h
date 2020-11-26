/**
 * Project: Implementation of a compiler of the IFJ20 language.
 * @brief   This is a header file for token_structure.c file. It includes heads of functions and definitions of constants.
 * @author  Robin Stancl <xstanc09@stud.fit.vutbr.cz>
 */

#ifndef IFJ_PROJEKT_TOKEN_STRUCTURE_H
#define IFJ_PROJEKT_TOKEN_STRUCTURE_H

#include <stdbool.h>
#include "string_functions.h"

/**
 * Definition of constants
 */

/**
 * Structures
 */
#include "string_functions.h"
#include "token_functions.h"

typedef struct token_list_itemT token_list_itemT;

///Structure for saving tokens
struct token_list_itemT
{
    tokenT token;           //Token itself
    token_list_itemT *next;      //Pointer to the next instruction int the list
};

///Structure list of tokens
typedef struct
{
    token_list_itemT *first;           //Pointer to the first token in the list
}token_listT;

/**
 * Heads of functions
 */
///Function for initialize list of tokens
void token_list_initialize(token_listT *list_of_tokens);

///Function for adding new token to the end of the list of tokens
bool token_list_add_item(token_listT *list_of_tokens, tokenT *token_to_add);

///Function for disposing the whole list of tokens
void token_list_free(token_listT *list_of_tokens);

#endif //IFJ_PROJEKT_TOKEN_STRUCTURE_H
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

///Structure for saving tokens
typedef struct tTkn
{
    tokenT token;           //Token itself
    struct tTkn *next;      //Pointer to the next instruction int the list
}*tToken;

///Structure list of tokens
typedef struct
{
    tToken First;           //Pointer to the first token in the list
}tListToken;

/**
 * Heads of functions
 */
///Function for initialize list of tokens
void list_token_initialize(tListToken *list_of_tokens);

///Function that mallocs token structure
tToken token_structure_init(tToken tkn, tokenT token_itself);

///Function for adding new token to the end of the list of tokens
bool add_token_to_list(tListToken *list_of_tokens, int token);

///Function for disposing the whole list of tokens
void dispose_list_of_tokens(tListToken *list_of_tokens);

#endif //IFJ_PROJEKT_TOKEN_STRUCTURE_H
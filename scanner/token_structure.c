/**
 * Project: Implementation of a compiler of the IFJ20 language.
 * @brief   This is an implementation of a functions for work with tokens in the list of tokens
 * @author  Robin Stancl <xstanc09@stud.fit.vutbr.cz>
 * @author  Dominik Vecera <xvecer23@stud.fit.vutbr.cz>
 */

#include <stdbool.h>
#include <stdlib.h>
#include "token_structure.h"
#include "scanner_functions.h"

///Function for initialize list of tokens
void list_token_initialize(tListToken *list_of_tokens)
{

    list_of_tokens->First = NULL;

}

tToken token_structure_init(tToken tkn, tokenT token_itself)
{
    if ((tkn = (tToken) malloc(sizeof(tToken) * sizeof(STRING_LENGTH))) != NULL) {
        tkn->token = token_itself;
        tkn->next = NULL;
        return tkn;
    }
}

///Function for adding new token to the end of the list of tokens
bool add_token_to_list(tListToken *list_of_tokens, int token)
{
    tToken tkn;
    if ((tkn = (tToken) malloc(sizeof(tToken) * sizeof(STRING_LENGTH))) != NULL)
    {
        if (list_of_tokens->First != NULL)
        {
            // Place new token at the end of the tokens list
            struct tTkn *last = list_of_tokens->First;
            while (last->next != NULL)
            {
                last = last->next;
            }

            // Store the token's information in it

            tkn->token = token;
            tkn->next = NULL;
            string_init(&tkn->token_string);
            string_copy(&tkn->token_string, &str);

            last->next = tkn;
        }
        else
        {
            // Store the token's information in it
            tkn->token = token;
            tkn->next = NULL;
            string_init(&tkn->token_string);
            string_copy(&tkn->token_string, &str);

            list_of_tokens->First = tkn;
        }
        return true;
    }
    else
    {
        return false; // memory allocation failed
    }
}

///Function for disposing the whole list of tokens
void dispose_list_of_tokens(tListToken *list_of_tokens)
{
    struct tTkn *tkn;

    while (list_of_tokens->First != NULL)
    {
        tkn = list_of_tokens->First;
        list_of_tokens->First = list_of_tokens->First->next;

        string_clear(&tkn->token_string);
        string_free(&tkn->token_string);
        token_array_free()

        free(tkn);
    }
}
/**
 * Project: Implementation of a compiler of the IFJ20 language.
 * @brief   This is an implementation of a functions for work with tokens in the list of tokens
 * @author  Robin Stancl <xstanc09@stud.fit.vutbr.cz>
 * @author  Dominik Vecera <xvecer23@stud.fit.vutbr.cz>
 * @author Boris Karavasilev <xkarav01@stud.fit.vutbr.cz>
 */

#include <stdbool.h>
#include <stdlib.h>
#include "token_linked_list.h"
#include "scanner_functions.h"

void token_list_item_set_token(token_list_itemT *new_item, tokenT *token);

/// Token list initialization
void token_list_initialize(token_listT *list_of_tokens)
{
    list_of_tokens->first = NULL;
}

/// Adds item to token linked list
bool token_list_add_item(token_listT *list_of_tokens, tokenT *token_to_add)
{
    // List is empty
    if (list_of_tokens->first == NULL) {
        if ((list_of_tokens->first = (token_list_itemT*) malloc(sizeof(token_list_itemT))) != NULL) {
            token_list_item_set_token(list_of_tokens->first, token_to_add);
        }
    } else {
        // Not empty

        token_list_itemT *token_list_last_item = NULL;
        token_list_itemT *current_item = list_of_tokens->first;

        while(current_item->next != NULL) {
            token_list_last_item = current_item;
            current_item = current_item->next;
        }

        if ((token_list_last_item->next = (token_list_itemT*) malloc(sizeof(token_list_itemT))) != NULL) {
            token_list_item_set_token(token_list_last_item->next, token_to_add);
        }
    }
}

// Private function
/// Copies token content into list item
void token_list_item_set_token(token_list_itemT *new_item, tokenT *token) {
    // Allocate memory for new item
    if (new_item != NULL) {
        // Initialize and copy content
        token_init(&new_item->token);
        copy_token(&new_item->token, token);
        new_item->next = NULL;
    }
}

/// Function for disposing the whole list of tokens
void token_list_free(token_listT *list_of_tokens)
{
    token_list_itemT *list_item_to_free = list_of_tokens->first;
    token_list_itemT *next_list_item;

    while(list_item_to_free != NULL) {
        next_list_item = list_item_to_free->next;
        token_free(&list_item_to_free->token);
        free(list_item_to_free);
        list_item_to_free = next_list_item;
    }
}

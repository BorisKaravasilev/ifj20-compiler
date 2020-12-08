/**
 * Project: Implementation of a compiler of the IFJ20 language.
 *
 * @file stack.c
 * @brief Stack implementation for semantic analysis.
 * @author Dominik Vecera <xvecer23@stud.fit.vutbr.cz>
 * @date 25. 10. 2020
 */

#include <stdlib.h>
#include "stack.h"

void stack_init(Stack *s){
    if (s != NULL){
        s->top = NULL;
        s->size = 0;
    }
}

bool stack_empty(Stack *s){
    return (s->top == NULL || s->size == 0);
}

stack_item stack_top(Stack *s){
    return *(s->top);
}

bool stack_push(Stack *s){
    if (s != NULL){
        stack_item *tmp = (stack_item *) malloc(sizeof(stack_item));
        if (tmp != NULL){
            tmp->symtable = st_init();
            tmp->next = s->top;

            s->top = tmp;
            s->size++;

            return true;

        } else
            return false; // compiler internal error 99
    }
    return false; // error
}

void stack_pop(Stack *s){
    if (s != NULL && !stack_empty(s)){
        stack_item *tmp;
        tmp = s->top;
        s->top = s->top->next;
        st_clear_all(tmp->symtable);
        s->size--;
    }
}

void stack_free(Stack *s){
    stack_item *temp = NULL;

    while (s->top != NULL) {
        temp = s->top;
        s->top = s->top->next;
        st_clear_all(temp->symtable);
        free(temp);
    }
}

ST_Item* stack_search(Stack* stack, stringT* key) {
    if (stack == NULL) {
        return NULL;
    }

    stack_item *actual = stack->top;
    while(actual != NULL) {
        ST_Item *search_item = st_search(actual->symtable, key);

        if (search_item != NULL) {
            return search_item;
        } else {
            actual = actual->next;
        }
    }
    return NULL;
}

Data_type st_get_type (Stack* stack, stringT* key) {
    /*If pointer at table is valid*/
    if (stack != NULL) {

        /*We create a temporary variable for searched symbol*/
        ST_Item *searched = stack_search(stack, key);

        if (searched != NULL) { /*If element was found we return it's type*/
            return searched->type;
        } else { /*If element is not present in the table we return NULL*/
            return TYPE_ERROR;
        }
    } else {
        return TYPE_ERROR;
    }
}
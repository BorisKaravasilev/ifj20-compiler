/**
 * Project: Implementation of a compiler of the IFJ20 language.
 *
 * @file stack.h
 * @brief Stack implementation for semantic analysis.
 * @author Dominik Večeřa <xvecer23@stud.fit.vutbr.cz>
 * @date 25. 10. 2020
 */

#ifndef IFJ_PROJEKT_STACK_H
#define IFJ_PROJEKT_STACK_H

#include <stdbool.h>
#include "symtable.h"

/**
 * @brief Structure of a stack element.
 */
typedef struct StackElem{
    Symtable *symtable;
    struct StackElem *next;
} stack_item;

/**
 * @brief Structure of the stack.
 */
typedef struct {
    stack_item *top;
    int size;
} Stack;

/**
 * @brief Initialization of the stack.
 * @param s Stack to be initiated.
 */
void stack_init(Stack *s);

/**
 * @brief Returns true or false depending on whether the stack is empty.
 * @param s Stack to be checked.
 * @return Truth value based on emptiness of stack.
 */
bool stack_empty(Stack *s);

/**
 * @brief Returns the token at the top of the stack.
 * @param s Stack from which the top element is taken.
 * @return Element at the top of the stack.
 */
stack_item stack_top(Stack *s);

/**
 * @brief Returns the searched symbol item in the table stack.
 * @param stack searched Stack.
 * @param searched item key
 * @return Symbol table item.
 */
ST_Item* stack_search(Stack* stack, stringT* key);

/**
 * Function that returns Symbol type.
 * @param pointer to symbol
 * @return Returns symbol data type
 */
Data_type st_get_type (Stack* stack, stringT* key);

/**
 * @brief Push a token to the token stack.
 * @param s Stack to be pushed to.
 * @param token The token to be pushed to the stack.
 * @return Truth value based on success of push operation.
 */
bool stack_push(Stack *s, Symtable *symtable);

/**
 * @brief Pop a token from the stack.
 * @param s Stack from which the token is to be popped.
 */
void stack_pop(Stack *s);

/**
 * @brief Empty stack by popping all of its elements from it.
 * @param s Stack to be emptied.
 */
void stack_free(Stack *s);

#endif //IFJ_PROJEKT_STACK_H
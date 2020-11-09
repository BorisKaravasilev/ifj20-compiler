//
// Created by Petr Vrtal on 26.10.2020.
//

#ifndef IFJ20_COMPILER_SEMANTIC_LATE_CHECK_H
#define IFJ20_COMPILER_SEMANTIC_LATE_CHECK_H

#define SEMANTIC_LATE_CHECK_MALLOC_ERROR 99

#include <stdbool.h>
#include "../scanner/symtable.h"

/**
 * @brief Structure of a stack element.
 */
typedef struct method_param_struct {
    int index;
    Data_type data_type;
    struct method_param_struct *next;
} method_param_structT;

/**
 * @brief Structure of a stack element.
 */
typedef struct late_check_stackElem {
    stringT method_name;
    int parameters_count;
    int return_types_count;
    method_param_structT *parameters_list_first;
    method_param_structT *return_types_list_first;
    struct late_check_stackElem *next;
} late_check_stack_item;

/**
 * @brief Structure of the stack.
 */
typedef struct {
    late_check_stack_item *top;
    int size;
} late_check_stack;

/**
 * @brief Initialization of the stack.
 * @param s Stack to be initiated.
 */
void late_check_stack_init(late_check_stack *s);

/**
 * @brief Returns true or false depending on whether the stack is empty.
 * @param s Stack to be checked.
 * @return Truth value based on emptiness of stack.
 */
bool late_check_stack_empty(late_check_stack *s);

/**
 * @brief Returns the token at the top of the stack.
 * @param s Stack from which the top element is taken.
 * @return Element at the top of the stack.
 */
late_check_stack_item late_check_stack_top(late_check_stack *s);

/**
 * @brief Push a token to the token stack.
 * @param s Stack to be pushed to.
 * @param token The token to be pushed to the stack.
 * @return Truth value based on success of push operation.
 */
bool late_check_stack_push_method(late_check_stack *s, stringT *name);

/**
 * @brief Pop a token from the stack.
 * @param s Stack from which the token is to be popped.
 */
void late_check_stack_pop(late_check_stack *s);

/**
 * @brief Empty stack by popping all of its elements from it.
 * @param s Stack to be emptied.
 */
void late_check_stack_free(late_check_stack *s);

/**
 * @brief Empty stack by popping all of its elements from it.
 * @param s Stack to be emptied.
 */
late_check_stack_item* late_check_stack_search(late_check_stack *s, stringT *method_name);

/**
 * @brief Creates function parameters and chains them in the stack_item
 * @param param_type type of parameter based on the enum
 * @param data_type Data type of the function parameter or return value
 */
void late_check_stack_item_add_parameter(late_check_stack_item *item, Data_type data_type);

/**
 * @brief Creates function parameters and chains them in the stack_item
 * @param param_type type of parameter based on the enum
 * @param data_type Data type of the function parameter or return value
 */
void late_check_stack_item_add_return_type(late_check_stack_item *item, Data_type data_type);

#endif //IFJ20_COMPILER_SEMANTIC_LATE_CHECK_H

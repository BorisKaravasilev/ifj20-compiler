/**
 * Project: Implementation of a compiler of the IFJ20 language.
 *
 * @file semantic_late_check.h
 * @brief Stack implementation for late semantic analysis.
 * @author Petr Vrtal <xvrtal01@stud.fit.vutbr.cz>
 * @date 26. 10. 2020
 */

#ifndef IFJ20_COMPILER_SEMANTIC_LATE_CHECK_H
#define IFJ20_COMPILER_SEMANTIC_LATE_CHECK_H

#define SEMANTIC_LATE_CHECK_MALLOC_ERROR 99

#include <stdbool.h>
#include "symtable.h"
#include "stack.h"
#include "return_codes.h"
#include "semantic_functions.h"

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
 * @brief Returns the late check stack item at the top of the stack.
 * @param s Stack from which the top element is taken.
 * @return Element at the top of the stack.
 */
late_check_stack_item late_check_stack_top(late_check_stack *s);

/**
 * @brief Push a token to the token stack.
 * @param s Stack to be pushed to.
 * @param name method name
 * @return Boolean value based on success of push operation.
 */
bool late_check_stack_push_method(late_check_stack *s, stringT *name);

/**
 * @brief Pop a late check stack item from the stack.
 * @param s Stack from which the token is to be popped.
 */
void late_check_stack_pop(late_check_stack *s);

/**
 * @brief Executes check for param list equality
 * @param late_check_param_list Late check parameters list
 * @param symtable_param_list Symtable parameters list
 * @return Boolean value based match of both sides
 */
bool param_lists_match(method_param_structT *late_check_param_list, st_function_data_param_structT *symtable_param_list);

/**
 * @brief Executes check for return types list equality
 * @param late_check_param_list Late check parameters list
 * @param symtable_param_list Symtable parameters list
 * @return Boolean value based match of both sides
 */
bool return_lists_match(method_param_structT *late_check_param_list, st_function_data_param_structT *symtable_param_list);

/**
 * @brief Executes function declaration semantic check for every function call in stack
 * @param late_check_s Late check stack with function call records
 * @param global_symtable Global symtable in which to search for function declaration
 */
int check_semantic_for_methods_call(late_check_stack *late_check_s, Stack *st_stack);

/**
 * @brief Frees late check stack item structure
 * @param item Late check stack item
 */
void late_check_stack_item_free(late_check_stack_item *item);

/**
 * @brief Dealloc method_param_struct list by freeing all of its elements.
 * @param method_param Method param to be de-allocated
 */
void late_check_method_param_list_free(method_param_structT *method_param);

/**
 * @brief Dealloc stack by freeing all of its elements.
 * @param s late check stack to be de-allocated.
 */
void late_check_stack_free(late_check_stack *s);

/**
 * @brief Searches and retrieves late check stack item based on its method name
 * @param s late check stack to search in.
 * @param method_name searched method name
 * @return late_check_stack item
 */
late_check_stack_item* late_check_stack_search(late_check_stack *s, stringT *method_name);

/**
 * @brief Creates late check stack item assignment list
 * @param item pointer at late_check_stack item
 * @param list pointer at left assignment list of params
 */
void late_check_stack_item_create_assignment_list(late_check_stack_item *item, assignment_paramT *list);

/**
 * @brief Creates function parameters and chains them in the stack_item
 * @param item pointer at late_check_stack item
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

/**
 * Project: Implementation of a compiler of the IFJ20 language.
 *
 * @file semantic_functions.h
 * @brief Implementation of semantic checking functions.
 * @author Petr Vrtal <xvrtal01@stud.fit.vutbr.cz>
 * @date 9. 11. 2020
 */

#ifndef IFJ20_COMPILER_SEMANTIC_FUNCTIONS_H
#define IFJ20_COMPILER_SEMANTIC_FUNCTIONS_H

#include "symtable.h"
#include "return_codes.h"

/**
 * @brief Structure for a assignment structure.
 */
typedef struct assignment_param_struct {
    int index;
    Data_type data_type;
    struct assignment_param_struct *next;
} assignment_paramT;

/**
 * @brief Structure of assigment element.
 */
typedef struct assignment_struct {
    int identifiers_count;
    int expressions_count;
    assignment_paramT *left_side_types_list_first;
    assignment_paramT *right_side_types_list_first;
} assignmentT;

/**
 * @brief Initialization of the assignment structure.
 * @param s Assignment structure to be initialized.
 */
void assignment_struct_init(assignmentT *s);

/**
 * @brief Returns true or false depending on whether the stack is empty.
 * @param s Assignment structure to check if empty.
 * @return Truth value based on emptiness of structure.
 */
bool assignment_struct_is_empty(assignmentT *s);

/**
 * @brief Empties the assignment structure.
 * @param s Assignment structure to empty.
 */
void assignment_struct_empty(assignmentT *s);

/**
 * @brief Empties and de-allocates assignment structure.
 * @param s Assignment structure to de-alloc.
 */
void assignment_struct_free(assignmentT *s);

/**
 * @brief Compares both sides of assignment.
 * @param s Assignment structure in which to compare left and right side param lists.
 */
bool compare_left_right_params(assignment_paramT *left, assignment_paramT *right);

/**
 * @brief Dealloc assignment_param_struct list by freeing all of its elements.
 * @param method_param Method param to be de-allocated
 */
void assignment_param_list_free(assignment_paramT *assignment_param);

/**
 * @brief Create and chain new assignment identifier on left side
 * @param item pointer at assignment structure
 * @param key Identifier name
 */
void assignment_add_identifier(assignmentT *item, stringT *key);

/**
 * @brief Create and chain new assignment expression on right side
 * @param item pointer at assignment structure
 * @param key Identifier name
 */
void assignment_add_expression(assignmentT *item, Data_type type);


#endif //IFJ20_COMPILER_SEMANTIC_FUNCTIONS_H

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
#include "../scanner/token_types.h"

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

typedef struct built_in_function_struct {
    char* function_name;
    int parameters_count;
    int return_types_count;
    char* parameter_types;
    char* return_types;
} built_in_functionT;

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
void compare_left_right_params(assignment_paramT *left, assignment_paramT *right);

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
void assignment_add_identifier(assignmentT *item, int token_type, ST_Item *st_item);

/**
 * @brief Create and chain new assignment expression on right side
 * @param item pointer at assignment structure
 * @param key Identifier name
 */
void assignment_add_expression(assignmentT *item, Data_type type);

static const built_in_functionT built_in_functions[10] = {
        {
                .function_name = "inputs",
                .parameters_count = 0,
                .return_types_count = 2,
                .parameter_types = "",
                .return_types = "si",
        },
        {
                .function_name = "inputi",
                .parameters_count = 0,
                .return_types_count = 2,
                .parameter_types = "",
                .return_types = "ii",
        },
        {
                .function_name = "inputf",
                .parameters_count = 0,
                .return_types_count = 2,
                .parameter_types = "",
                .return_types = "fi",
        },
        {
                .function_name = "print",
                .parameters_count = -1, // if param count -1 then function accepts 0 to n terms
                .return_types_count = 0,
                .parameter_types = "",
                .return_types = "",
        },
        {
                .function_name = "int2float",
                .parameters_count = 1,
                .return_types_count = 1,
                .parameter_types = "i",
                .return_types = "f",
        },
        {
            .function_name = "float2int",
            .parameters_count = 1,
            .return_types_count = 1,
            .parameter_types = "f",
            .return_types = "i",
        },
        {
            .function_name = "len",
            .parameters_count = 1,
            .return_types_count = 1,
            .parameter_types = "s",
            .return_types = "i",
        },
        {
            .function_name = "substr",
            .parameters_count = 3,
            .return_types_count = 2,
            .parameter_types = "sii",
            .return_types = "si",
        },
        {
            .function_name = "ord",
            .parameters_count = 2,
            .return_types_count = 2,
            .parameter_types = "si",
            .return_types = "ii",
        },
        {
            .function_name = "chr",
            .parameters_count = 1,
            .return_types_count = 2,
            .parameter_types = "i",
            .return_types = "si",
        },
};

#endif //IFJ20_COMPILER_SEMANTIC_FUNCTIONS_H

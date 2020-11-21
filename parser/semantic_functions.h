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

#include <string.h>
#include "symtable.h"
#include "return_codes.h"
#include "../scanner/token_types.h"

#define BUILT_IN_FUNCTIONS_COUNT 10

/**
 * @brief Structure for a assignment structure.
 */
typedef struct assignment_param_struct {
    int index;
    ST_Item *symbol;
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
    int function_token_type;
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
 * @brief Derives id data type on left side from expression on righ side in the assignment structure.
 * @param s Assignment structure.
 */
void assignment_derive_id_type(assignmentT *s);

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
void assignment_add_expression(assignmentT *item, Data_type type, ST_Item *symbol);

/**
 * @brief Create and chain new data types on right side based on user function
 * @param item pointer at assignment structure
 * @param function Symtable item
 */
void assignment_add_user_function(assignmentT *item, ST_Item *function);

/**
 * @brief Create and chain new data types on right side based on built in function
 * @param item pointer at assignment structure
 * @param function Symtable item
 */
void assignment_add_built_in_function(assignmentT *item, int function_token);

static const built_in_functionT built_in_functions[BUILT_IN_FUNCTIONS_COUNT] = {
        {
                .function_token_type = TOKEN_FUNCTION_INPUTS,
                .parameters_count = 0,
                .return_types_count = 2,
                .parameter_types = "",
                .return_types = "si",
        },
        {
                .function_token_type = TOKEN_FUNCTION_INPUTI,
                .parameters_count = 0,
                .return_types_count = 2,
                .parameter_types = "",
                .return_types = "ii",
        },
        {
                .function_token_type = TOKEN_FUNCTION_INPUTF,
                .parameters_count = 0,
                .return_types_count = 2,
                .parameter_types = "",
                .return_types = "fi",
        },
        {
                .function_token_type = TOKEN_FUNCTION_PRINT,
                .parameters_count = -1, // if param count -1 then function accepts 0 to n terms
                .return_types_count = 0,
                .parameter_types = "",
                .return_types = "",
        },
        {
                .function_token_type = TOKEN_FUNCTION_INT2FLOAT,
                .parameters_count = 1,
                .return_types_count = 1,
                .parameter_types = "i",
                .return_types = "f",
        },
        {
            .function_token_type = TOKEN_FUNCTION_FLOAT2INT,
            .parameters_count = 1,
            .return_types_count = 1,
            .parameter_types = "f",
            .return_types = "i",
        },
        {
            .function_token_type = TOKEN_FUNCTION_LEN,
            .parameters_count = 1,
            .return_types_count = 1,
            .parameter_types = "s",
            .return_types = "i",
        },
        {
            .function_token_type = TOKEN_FUNCTION_SUBSTR,
            .parameters_count = 3,
            .return_types_count = 2,
            .parameter_types = "sii",
            .return_types = "si",
        },
        {
            .function_token_type = TOKEN_FUNCTION_ORD,
            .parameters_count = 2,
            .return_types_count = 2,
            .parameter_types = "si",
            .return_types = "ii",
        },
        {
            .function_token_type = TOKEN_FUNCTION_CHR,
            .parameters_count = 1,
            .return_types_count = 2,
            .parameter_types = "i",
            .return_types = "si",
        },
};

#endif //IFJ20_COMPILER_SEMANTIC_FUNCTIONS_H

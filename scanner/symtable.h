/**
 * Project: Implementation of a compiler of the IFJ20 language.
 * @file    symtable.h
 * @brief   Header file for symtable.h
 * @author  Petr Vrtal <xvrtal01@stud.fit.vutbr.cz>
 * @date    25.10.2020
 */

#ifndef IFJ_PROJEKT_SYMTABLE_H
#define IFJ_PROJEKT_SYMTABLE_H

#define MAX_SYMTABLE_SIZE 101

#define SYMTABLE_MALLOC_ERROR 99

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "string_functions.h"
#include "return_codes.h"

/**
 * @brief Enum of all possible Symbol data types
 */
typedef enum {
    TYPE_ERROR,
    TYPE_INT,
    TYPE_DECIMAL,
    TYPE_STRING,
    TYPE_NIL,
    TYPE_BLANK_VARIABLE,
} Data_type;

/**
 * @brief Symtable function data parameter structure.
 */
typedef struct st_function_data_param_struct {
    int index;
    Data_type data_type;
    struct st_function_data_param_struct *next;
} st_function_data_param_structT;

/**
 * @brief Symtable function data structure.
 */
typedef struct st_item_function_struct {
    bool defined;
    int parameters_count;
    int return_types_count;
    st_function_data_param_structT *parameters_list_first;
    st_function_data_param_structT *return_types_list_first;
} st_item_function_structT;

/**
 * @brief Symbol table item structure
 */
typedef struct tST_Item {
    stringT key;
    Data_type type;
    stringT content;
    st_item_function_structT *function_data;
    struct tST_Item *next;
} ST_Item;

/**
 * @brief Symbol table structure
 */
typedef struct symTable {
    int size;
    ST_Item *items[MAX_SYMTABLE_SIZE];
} Symtable;

/**
 * Bernstein's hash function djb2
 * @param symbol key
 * @link http://www.cse.yorku.ca/~oz/hash.html
 * @return returns unsigned long hash
 */
unsigned long st_hash (unsigned char* key);

/**
 * Initialize Symbol table.
 * @param returns symtable
 */
Symtable* st_init ();

/**
 * Creates symtable item.
 * @return returns symtable item
 */
ST_Item* st_create_item ();

/**
 * Search symbol in symtable.
 * @param ptr_symtable pointer at symtable structure
 * @param key identifier key
 * @return returns pointer at symtable item
 */
ST_Item* st_search (Symtable* ptr_symtable, stringT* key);

/**
 * Inserts new symbol to symbol table.
 * @param pointer to symbol table
 * @param new symbol key
 * @param pointer to new symbol data
 * @return returns pointer at symtable item
 */
ST_Item* st_insert_symbol (Symtable* ptr_symtable, stringT* key, bool function);

/**
 * Changes defined parameter in the symbol.
 * @param pointer to symbol table
 * @param symbol key
 * @param defined state bool value
 * @return returns pointer at symtable item
 */
ST_Item* st_item_change_defined(Symtable* ptr_symtable, stringT* key, bool defined);

/**
 * Checks whether supplied symtable item represents a function or not.
 * @param pointer at symtable item
 * @return returns true if symbol item represents a function
 */
bool st_item_is_function(ST_Item *item);

/**
 * Adds new parameter to the function symbol.
 * @param pointer to symbol data
 * @param symbol data type
 * @return true if adding was successful, otherwise false
 */
int st_add_function_param (ST_Item* symbol, Data_type type);

/**
 * Adds new return type to the function symbol.
 * @param pointer to symbol data
 * @param symbol data type
 * @return true if adding was successful, otherwise false
 */
int st_add_function_return_type (ST_Item* symbol, Data_type type);

/**
 * Function that returns Symbol table item content.
 * @param pointer to symbol table
 * @param searched item key
 * @return Returns symtable item content
 */
char* st_get_content (Symtable* ptr_symtable, stringT* key);

/**
 * Function that returns Symbol table item function params.
 * @param pointer to symbol table
 * @param searched item key
 * @return Returns table item params
 */
st_function_data_param_structT* st_get_function_params (Symtable* ptr_symtable, stringT* key);

/**
 * Function that returns Symbol table item function params.
 * @param pointer to symbol table
 * @param searched item key
 * @return Returns table item params
 */
st_function_data_param_structT* st_get_function_return_types (Symtable* ptr_symtable, stringT* key);

/**
 * Delete Symbol table item.
 * @param pointer to symbol table
 * @param searched table item key
 */
/*void st_delete_symbol (Symtable* ptr_symtable, stringT *key);*/

/**
 * @brief Dealloc function_data_param_struct list by freeing all of its elements.
 * @param method_param Method param to be de-allocated
 */
void st_function_param_list_free(st_function_data_param_structT *method_param);

/**
 * @brief Dealloc function_data_struct list by freeing all of its elements.
 * @param function_data function data struct to be de-allocated
 */
void st_function_data_free(st_item_function_structT *function_data);

/**
 * Clears entire item list in symbol.
 * @param pointer to symbol table
 */
void st_clear_items_list (ST_Item* item);

/**
 * Clears entire Symbol table.
 * @param pointer to symbol table
 */
void st_clear_all (Symtable* ptr_symtable);

#endif //IFJ_PROJEKT_SYMTABLE_H

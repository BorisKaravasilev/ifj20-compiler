/**
 * Project: Implementation of a compiler of the IFJ20 language.
 * @file    symtable.c
 * @brief   Implementation of Symbol table.
 * @author  Petr Vrtal <xvrtal01@stud.fit.vutbr.cz>
 * @date    25.10.2020
 */

#ifndef IFJ_PROJEKT_SYMTABLE_H
#define IFJ_PROJEKT_SYMTABLE_H

#define MAX_SYMTABLE_SIZE 256

#define SYMTABLE_MALLOC_ERROR 99

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../general/string_functions.h"

/**
 * @brief Enum of all possible Symbol data types
 */
typedef enum {
    TYPE_ERROR,
    TYPE_INT,
    TYPE_DECIMAL,
    TYPE_STRING,
    TYPE_NIL
} Data_type;

/**
 * @brief Symbol table item structure
 */
typedef struct tST_Item {
    stringT* key;
    Data_type type;
    stringT* content;
    int function;
    int defined;
    stringT* params;
    struct tST_Item* next;
} ST_Item;

/**
 * @brief Symbol table structure
 */
typedef struct symTable {
    int size;
    ST_Item *items[MAX_SYMTABLE_SIZE];
} Symtable;

/**
 * Elf Hash hashing function used as hashing algorithm
 * @param symbol key
 * @link https://en.wikipedia.org/wiki/PJW_hash_function
 * @return returns unsigned long hash
 */
unsigned long st_hash (stringT* key);

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

ST_Item* st_search (Symtable* ptr_symtable, stringT* key);

/**
 * Inserts new symbol to symbol table.
 * @param pointer to symbol table
 * @param new symbol key
 * @param pointer to new symbol data
 */
ST_Item* st_insert_symbol (Symtable* ptr_symtable, stringT* key, int function);

/**
 * Changes defined parameter in the symbol.
 * @param pointer to symbol table
 * @param new symbol key
 * @param defined state integer
 * @return true if adding was successful, otherwise false
 */
ST_Item* st_item_change_defined(Symtable* ptr_symtable, stringT* key, int defined);

/**
 * Adds new parameter to the function symbol.
 * @param pointer tosymbol data
 * @param symbol data type
 * @return true if adding was successful, otherwise false
 */
int st_add_param (ST_Item* symbol, Data_type type);

/**
 * Function that returns Symbol table item content.
 * @param pointer to symbol table
 * @param searched item key
 * @return Returns table item content
 */
char* st_get_content (Symtable* ptr_symtable, stringT* key);

/**
 * Function that returns Symbol table item params.
 * @param pointer to symbol table
 * @param searched item key
 * @return Returns table item params
 */
char* st_get_params (Symtable* ptr_symtable, stringT* key);

/**
 * Delete Symbol table item.
 * @param pointer to symbol table
 * @param searched table item key
 */
/*void st_delete_symbol (Symtable* ptr_symtable, stringT *key);*/

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

//
// Created by Boris on 14-Oct-20.
//

#ifndef IFJ20_COMPILER_SYMTABLE_H
#define IFJ20_COMPILER_SYMTABLE_H

#define SYMTABLE_INIT_LEN 5

#include <stdbool.h>
#include "../general/string_functions.h"

// Holds data about identifiers
typedef struct symtable_item {
    char *ptr_string; // TODO: Change to string
    int len;
    int type;
    bool used;
    struct symtable_item *next; // For collisions (malloc)
    // Next data in sym table
} symtable_itemT;

// Symbol table (Table of identifiers)
typedef struct {
    symtable_itemT *ptr_item; // Pointer to first item
    int length;
    int alloc_size;
} symtableT;

int symtable_init(symtableT *ptr_symtable);
int symtable_add_item(symtableT *ptr_symtable, char *ptr_string);
void symtable_free(symtableT *ptr_symtable);
void debug_whole_symtable(symtableT *ptr_symtable);

#endif //IFJ20_COMPILER_SYMTABLE_H

//
// Created by Boris on 14-Oct-20.
//

#ifndef IFJ20_COMPILER_SYMTABLE_H
#define IFJ20_COMPILER_SYMTABLE_H

#include <stdbool.h>
#include "../general/string_functions.h"

// Holds data about identifiers
typedef struct {
    stringT *string_val;
    int len;
    int type;
    bool used;
} symtable_itemT;

// Symbol table (Table of identifiers)
typedef struct {
    symtable_itemT *item; // Pointer to first item
    int alloc_len;
} symtableT;

#endif //IFJ20_COMPILER_SYMTABLE_H

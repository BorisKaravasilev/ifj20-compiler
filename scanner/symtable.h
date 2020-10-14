//
// Created by Boris on 14-Oct-20.
//

#ifndef IFJ20_COMPILER_SYMTABLE_H
#define IFJ20_COMPILER_SYMTABLE_H

#include <stdbool.h>

typedef struct {
    char *string_val;
    int len;
    int type;
    bool used;
} symtable_itemT;

typedef struct {
    int alloc_len;
} symtableT;

#endif //IFJ20_COMPILER_SYMTABLE_H

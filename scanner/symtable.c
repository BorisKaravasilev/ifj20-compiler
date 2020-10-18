//
// Created by Boris on 14-Oct-20.
//

#include <stdlib.h>
#include "symtable.h"
#include "../general/debugging.h"

int symtable_init(symtableT *ptr_symtable) {
    size_t init_alloc_size = SYMTABLE_INIT_LEN * sizeof(symtable_itemT);

    ptr_symtable->ptr_item = NULL;
    ptr_symtable->length = 0;
    ptr_symtable->alloc_size = 0;

    if ((ptr_symtable->ptr_item = (symtable_itemT *) malloc(init_alloc_size)) == NULL)
        return 1;

    ptr_symtable->alloc_size = init_alloc_size;

    return 0;
}

int symtable_add_item(symtableT *ptr_symtable, char *ptr_string) {
    if (ptr_symtable->length + 1 >= ptr_symtable->alloc_size)
    {
        size_t new_alloc_size = ptr_symtable->alloc_size;
        new_alloc_size += SYMTABLE_INIT_LEN * sizeof(symtable_itemT);

        if ((ptr_symtable->ptr_item = (symtable_itemT *) realloc(ptr_symtable->ptr_item, new_alloc_size)) == NULL)
        {
            return 1;
        }

        ptr_symtable->alloc_size = new_alloc_size;
    }

    ptr_symtable->ptr_item[ptr_symtable->length].ptr_string = ptr_string;
    ptr_symtable->length += 1;
    return 0;
}

void symtable_free(symtableT *ptr_symtable) {
    if (ptr_symtable != NULL) {
        free(ptr_symtable->ptr_item);
        ptr_symtable->length = 0;
        ptr_symtable->alloc_size = 0;
    }
}

void debug_whole_symtable(symtableT *ptr_symtable) {
    debug_scanner("\n\nSymtable content\n====================\n%s", ""); // Cant put zero param because of macro in c99
    // Display whole symbol table
    for (int i = 0; i < ptr_symtable->length; ++i) {
        debug_scanner("SYMTABLE item[%d]: \"%s\"\n", i, ptr_symtable->ptr_item[i].ptr_string);
    }
}
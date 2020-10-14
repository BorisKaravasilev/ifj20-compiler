//
// Created by Boris on 14-Oct-20.
//

#include <stdlib.h>
#include "symtable.h"

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
        free(ptr_symtable);
        ptr_symtable->length = 0;
        ptr_symtable->alloc_size = 0;
    }
}

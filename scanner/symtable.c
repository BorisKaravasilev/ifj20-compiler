/**
 * Project: Implementation of a compiler of the IFJ20 language.
 * @file    symtable.c
 * @brief   Implementation of Symbol table.
 * @author  Petr Vrtal <xvrtal01@stud.fit.vutbr.cz>
 * @date    25.10.2020
 */

#include "symtable.h"

unsigned long st_hash (stringT* key) {
    int hash = 503;
    char* temp;

    temp = malloc(key->alloc_size);
    strcpy(temp, key->string);

    while (*temp != '\0') {
        hash = ((hash << 4) + (int)(*temp)) % MAX_SYMTABLE_SIZE;
        temp++;
    }


    //free(temp);

    return hash % MAX_SYMTABLE_SIZE;
}

Symtable* st_init () {
    if (MAX_SYMTABLE_SIZE < 1) {
        return NULL;
    }

    Symtable* ptr_symtable = malloc(sizeof(Symtable));

    /*If symtable ptr is not null we proceed*/
    if (ptr_symtable == NULL) {
        exit(SYMTABLE_MALLOC_ERROR);
    }

    int i;
    /*We set every symtable array item to be NULL*/
    for (i = 0; i < MAX_SYMTABLE_SIZE; i++) {
        ptr_symtable->items[i] = NULL;
    }

    ptr_symtable->size = MAX_SYMTABLE_SIZE;
    return ptr_symtable;
}

ST_Item* st_create_item () {
    ST_Item* temp = NULL;

    if ((temp = malloc(sizeof(ST_Item))) == NULL) {
        exit(SYMTABLE_MALLOC_ERROR);
    }

    temp->function = 0;
    temp->defined = 0;
    temp->next = NULL;

    return temp;
}

ST_Item* st_search (Symtable* ptr_symtable, stringT* key) {

    /*If symtable ptr is not NULL we proceed*/
    if (ptr_symtable != NULL) {

        /*We create a temporary variable for Item*/
        ST_Item *temp = ptr_symtable->items[st_hash(key)];

        if (temp != NULL) {
            /*We move through chained symbols until we get NULL*/
            while (string_compare(&temp->key, key) != 0) {
                if (temp->next == NULL) {
                    return NULL;
                } else {
                   temp = temp->next;
                }
            }
        }
        return temp;
    } else {
        return NULL;
    }
}

ST_Item* st_insert_symbol (Symtable* ptr_symtable, stringT* key, int function) {

    /*If symtable ptr is NULL we return*/
    if (ptr_symtable == NULL) {
        return NULL;
    }

    ST_Item* temp = NULL;
    ST_Item* new_item;

    temp = st_search(ptr_symtable, key);

    if (temp != NULL) {
        return NULL;
    } else {
        new_item = st_create_item();
        if (new_item == NULL) {
            return NULL;
        }

        string_init(&new_item->key);
        string_init(&new_item->content);
        string_init(&new_item->params);

        temp = ptr_symtable->items[st_hash(key)];
        string_copy(&new_item->key, key);

        new_item->type = TYPE_NIL;
        new_item->function = function;
        new_item->defined = 0;

        if (temp != NULL) {
            while (temp != NULL) {
                if (temp->next == NULL) {
                    break;
                } else {
                    temp = temp->next;
                }
            }

            temp->next = new_item;
        } else {
            ptr_symtable->items[st_hash(key)] = new_item;
        }
    }
    return new_item;
}

ST_Item* st_item_change_defined(Symtable* ptr_symtable, stringT* key, int defined) {
    /*If symtable ptr is NULL we return*/
    if (ptr_symtable == NULL) {
        return NULL;
    }

    ST_Item* temp = st_search(ptr_symtable, key);
    if (temp == NULL) {
        return NULL;
    } else {
        temp->defined = defined;
        return temp;
    }
}

int st_add_param (ST_Item* symbol, Data_type type) {

    /*If symbol ptr is NULL we return false*/
    if (symbol == NULL) {
        return 1;
    }

    switch (type) {
        case TYPE_INT:
            if ((string_add_character(&symbol->params, 'i')) == 1)
                return 1;
            break;

        case TYPE_DECIMAL:
            if ((string_add_character(&symbol->params, 'd')) == 1)
                return 1;
            break;

        case TYPE_STRING:
            if ((string_add_character(&symbol->params, 's')) == 1)
                return 1;
            break;

        default:
            break;
    }

    return 0;
}

char* st_get_content (Symtable* ptr_symtable, stringT* key) {

    /*If pointer at table is valid*/
    if (ptr_symtable != NULL) {

        /*We create a temporary variable for searched symbol*/
        ST_Item *searched = st_search(ptr_symtable, key);

        if (searched != NULL) { /*If element was found we return it's data*/
            return string_get(&searched->content);
        } else { /*If element is not present in the table we return NULL*/
            return NULL;
        }
    } else {
        return NULL;
    }
}

char* st_get_params (Symtable* ptr_symtable, stringT* key) {

    /*If pointer at table is valid*/
    if (ptr_symtable != NULL) {

        /*We create a temporary variable for searched symbol*/
        ST_Item *searched = st_search(ptr_symtable, key);

        if (searched != NULL) { /*If element was found we return it's data*/
            return string_get(&searched->params);
        } else { /*If element is not present in the table we return NULL*/
            return NULL;
        }
    } else {
        return NULL;
    }
}

void st_clear_items_list (ST_Item* item) {

    if (item != NULL) {
        ST_Item* next = item->next;

        string_free(&item->content);
        string_free(&item->params);
        string_free(&item->key);
        free(item);

        ST_Item* tmp;

        while (next != NULL){
            tmp = next;
            next = next->next;

            string_free(&tmp->content);
            string_free(&tmp->params);
            string_free(&tmp->key);
            free(tmp);
        }
    }
}

void st_clear_all (Symtable* ptr_symtable) {

    /*If pointer at table is valid*/
    if (ptr_symtable != NULL) {
        int i;
        for (i = 0; i < ptr_symtable->size; i++) {
            st_clear_items_list(ptr_symtable->items[i]);
            ptr_symtable->items[i] = NULL;
        }
        free(ptr_symtable);

    }
}


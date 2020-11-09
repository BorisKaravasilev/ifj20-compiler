/**
 * Project: Implementation of a compiler of the IFJ20 language.
 * @file    symtable.c
 * @brief   Implementation of Symbol table.
 * @author  Petr Vrtal <xvrtal01@stud.fit.vutbr.cz>
 * @date    25.10.2020
 */

#include "symtable.h"

// TODO: check return values

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

    temp->function_data = NULL;
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

ST_Item* st_insert_symbol (Symtable* ptr_symtable, stringT* key, bool function) {

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

        temp = ptr_symtable->items[st_hash(key)];
        string_copy(&new_item->key, key);

        new_item->type = TYPE_NIL;

        if (function && new_item->function_data == NULL) {
            if ((new_item->function_data = (st_item_function_structT *) malloc(sizeof(st_item_function_structT))) == NULL) {
                exit(SYMTABLE_MALLOC_ERROR);
            }
            new_item->function_data->defined = false;
            new_item->function_data->parameters_list_first = NULL;
            new_item->function_data->return_types_list_first = NULL;
        }

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

ST_Item* st_item_change_defined(Symtable* ptr_symtable, stringT* key, bool defined) {
    /*If symtable ptr is NULL we return*/
    if (ptr_symtable == NULL) {
        return NULL;
    }

    ST_Item* temp = st_search(ptr_symtable, key);
    if (temp == NULL) {
        return NULL;
    } else {
        if (temp->function_data != NULL) {
            temp->function_data->defined = defined;
        }

        return temp;
    }
}

int st_add_function_param (ST_Item* symbol, Data_type type) {

    /*If symbol ptr is NULL we return false*/
    if (symbol == NULL) {
        // TODO: change possible return value
        return 1;
    }

    if (symbol->function_data == NULL) {
        return 1;
    }

    st_function_data_param_structT *new_parameter;
    if ((new_parameter = (st_function_data_param_structT *) malloc(sizeof(st_function_data_param_structT))) == NULL) {
        exit(SYMTABLE_MALLOC_ERROR);
    }
    new_parameter->data_type = type;
    new_parameter->next = NULL;
    new_parameter->index = symbol->function_data->parameters_count++;

    if (symbol->function_data->parameters_list_first == NULL) {
        symbol->function_data->parameters_list_first = new_parameter;
    } else {
        st_function_data_param_structT *current = symbol->function_data->parameters_list_first;

        while (current->next != NULL) {
            current = current->next;
        }
        current->next = new_parameter;
    }

    return 0;
}

int st_add_function_return_type (ST_Item* symbol, Data_type type) {

    /*If symbol ptr is NULL we return false*/
    if (symbol == NULL) {
        // TODO: change possible return value
        return 1;
    }

    if (symbol->function_data == NULL) {
        return 1;
    }

    st_function_data_param_structT *new_parameter;
    if ((new_parameter = (st_function_data_param_structT *) malloc(sizeof(st_function_data_param_structT))) == NULL) {
        exit(SYMTABLE_MALLOC_ERROR);
    }
    new_parameter->data_type = type;
    new_parameter->next = NULL;
    new_parameter->index = symbol->function_data->return_types_count++;

    if (symbol->function_data->return_types_list_first == NULL) {
        symbol->function_data->return_types_list_first = new_parameter;
    } else {
        st_function_data_param_structT *current = symbol->function_data->return_types_list_first;

        while (current->next != NULL) {
            current = current->next;
        }
        current->next = new_parameter;
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

st_function_data_param_structT* st_get_function_params (Symtable* ptr_symtable, stringT* key) {

    /*If pointer at table is valid*/
    if (ptr_symtable != NULL) {

        /*We create a temporary variable for searched symbol*/
        ST_Item *searched = st_search(ptr_symtable, key);

        if (searched != NULL && searched->function_data != NULL) { /*If element was found we return it's data*/
            return searched->function_data->parameters_list_first;
        } else { /*If element is not present in the table we return NULL*/
            return NULL;
        }
    } else {
        return NULL;
    }
}

st_function_data_param_structT* st_get_function_return_types (Symtable* ptr_symtable, stringT* key) {

    /*If pointer at table is valid*/
    if (ptr_symtable != NULL) {

        /*We create a temporary variable for searched symbol*/
        ST_Item *searched = st_search(ptr_symtable, key);

        if (searched != NULL && searched->function_data != NULL) { /*If element was found we return it's data*/
            return searched->function_data->return_types_list_first;
        } else { /*If element is not present in the table we return NULL*/
            return NULL;
        }
    } else {
        return NULL;
    }
}

void st_function_param_list_free(st_function_data_param_structT *method_param) {
    if (method_param != NULL) {
        st_function_data_param_structT *temp;
        st_function_data_param_structT *current = method_param;

        while (current != NULL) {
            temp = current;
            current = current->next;
            free(temp);
        }
    }
}

void st_function_data_free(st_item_function_structT *function_data) {
    if (function_data != NULL) {
        st_function_param_list_free(function_data->parameters_list_first);
        st_function_param_list_free(function_data->return_types_list_first);
    }
}

void st_clear_items_list (ST_Item* item) {

    if (item != NULL) {
        ST_Item* next = item->next;

        string_free(&item->content);
        string_free(&item->key);
        st_function_data_free(item->function_data);
        free(item);

        ST_Item* tmp;

        while (next != NULL){
            tmp = next;
            next = next->next;

            string_free(&tmp->content);
            string_free(&tmp->key);
            st_function_data_free(item->function_data);
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


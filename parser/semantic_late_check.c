/**
 * Project: Implementation of a compiler of the IFJ20 language.
 *
 * @file semantic_late_check.c
 * @brief Stack implementation for late semantic analysis.
 * @author Petr Vrtal <xvrtal01@stud.fit.vutbr.cz>
 * @date 26. 10. 2020
 */

#include <stdlib.h>
#include "semantic_late_check.h"

void late_check_stack_init(late_check_stack *s){
    if (s != NULL){
        s->top = NULL;
        s->size = 0;
    }
}

bool late_check_stack_empty(late_check_stack *s){
    return (s->top == NULL || s->size == 0);
}

late_check_stack_item late_check_stack_top(late_check_stack *s){
    return *(s->top);
}

bool late_check_stack_push_method(late_check_stack *s, stringT *name){
    if (s != NULL){
        late_check_stack_item *tmp = (late_check_stack_item *) malloc(sizeof(late_check_stack_item));
        if (tmp != NULL){
            string_init(&tmp->method_name);
            string_copy(&tmp->method_name, name);

            tmp->parameters_list_first = NULL;
            tmp->return_types_list_first = NULL;

            tmp->next = s->top;

            s->top = tmp;
            s->size++;

            return true;

        } else
            return false; // compiler internal error 99
    }
    return false; // error
}

void late_check_stack_pop(late_check_stack *s){
    if (s != NULL && !late_check_stack_empty(s)){
        late_check_stack_item *tmp;
        tmp = s->top;
        s->top = s->top->next;
        late_check_stack_item_free(tmp);
        s->size--;
    }
}

bool param_lists_match(method_param_structT *late_check_param_list, st_function_data_param_structT *symtable_param_list) {
    method_param_structT *lc_current = late_check_param_list;
    st_function_data_param_structT *st_current = symtable_param_list;

    /* Check every parameter in the method_param list */
    while (lc_current != NULL || st_current != NULL) {
        if (lc_current == NULL || st_current == NULL) {
            return false;
        } else {
            /* If data types dont match we return false */
            if (lc_current->data_type != st_current->data_type) {
                return false;
            }
        }

        lc_current = lc_current->next;
        st_current = st_current->next;
    }

    return true;
}

bool return_lists_match(method_param_structT *late_check_param_list, st_function_data_param_structT *symtable_param_list) {
    method_param_structT *lc_current = late_check_param_list;
    st_function_data_param_structT *st_current = symtable_param_list;

    if (lc_current == NULL) {
        /* if method doesnt have return type its ok */
        return true;
    } else {
        /* Check every return type in the method_param list */
        while (lc_current != NULL || st_current != NULL) {
            if (lc_current == NULL || st_current == NULL) {
                return false;
            } else {
                /* if return types during function call are not the same for symtable item return types it is an error*/
                if (lc_current->data_type != st_current->data_type) {
                    /* if left identifier is underline its ok though */
                    if (lc_current->data_type != TYPE_BLANK_VARIABLE)
                        return false;
                }
            }

            lc_current = lc_current->next;
            st_current = st_current->next;
        }
    }

    return true;
}

int check_semantic_for_methods_call(late_check_stack *late_check_s, Stack *st_stack) {
    /* Check every function call and compares them with record in symtable */
    if (late_check_s != NULL && st_stack != NULL) {
        while (late_check_s->top != NULL) {
            /* find method in symtable */
            ST_Item *symbol = stack_search(st_stack, &late_check_s->top->method_name);
            if (symbol == NULL || !st_item_is_function(symbol)) {
                /* if its not there or its not a function its a semantic error */
                fprintf(stderr, "Error: Calling undefined function \'%s\'", late_check_s->top->method_name.string);
                return RC_SEMANTIC_IDENTIFIER_ERR;
            }
            if (param_lists_match(late_check_s->top->parameters_list_first, symbol->function_data->parameters_list_first) == false) {
                /* if supplied parameters do not match its semantic error */
                fprintf(stderr, "Error: Invalid parameters supplied to function \'%s\'", symbol->key.string);
                return RC_SEMANTIC_FUNC_PARAM_ERR;
            } else if (return_lists_match(late_check_s->top->return_types_list_first, symbol->function_data->return_types_list_first) == false) {
                /* if return types do not match its semantic error */
                fprintf(stderr, "Error: Invalid return types for function \'%s\'", symbol->key.string);
                return RC_SEMANTIC_FUNC_PARAM_ERR;
            }

            /* pop successfully checked function call from the stack */
            late_check_stack_pop(late_check_s);
        }
        return SEMANTIC_OK;
    }
}

void late_check_stack_item_free(late_check_stack_item *item) {
    if (item != NULL) {
        string_free(&item->method_name);
        late_check_method_param_list_free(item->parameters_list_first);
        late_check_method_param_list_free(item->return_types_list_first);
        free(item);
    }
}

void late_check_method_param_list_free(method_param_structT *method_param) {
    if (method_param != NULL) {
        method_param_structT *temp;
        method_param_structT *current = method_param;

        while (current != NULL) {
            temp = current;
            current = current->next;
            free(temp);
        }
    }
}

void late_check_stack_free(late_check_stack *s){
    late_check_stack_item *temp = NULL;

    while (s->top != NULL) {
        temp = s->top;
        s->top = s->top->next;

        late_check_stack_item_free(temp);
        free(temp);
    }
}

late_check_stack_item* late_check_stack_search(late_check_stack *s, stringT *method_name) {
    if (s == NULL || method_name == NULL) {
        return NULL;
    }

    late_check_stack_item *current = s->top;
    while (current != NULL) {
        if (string_compare(&current->method_name, method_name) == 0) {
            return current;
        }
        current = s->top->next;
    }
    return NULL;
}

void late_check_stack_item_create_assignment_list(late_check_stack_item *item, assignment_paramT *list) {
    if (item == NULL || list == NULL) {
        return;
    }

    assignment_paramT *current = list;
    while (current != NULL) {
        /* copies all identifiers from the list of left side assignment into the structure */
        late_check_stack_item_add_return_type(item, current->data_type);
        current = current->next;
    }
}

void late_check_stack_item_add_assignment_param(late_check_stack_item *item, Data_type data_type) {
    if (item == NULL) {
        return;
    }

    method_param_structT *new_parameter;
    if ((new_parameter = (method_param_structT *) malloc(sizeof(method_param_structT))) == NULL) {
        exit(SEMANTIC_LATE_CHECK_MALLOC_ERROR);
    }
    new_parameter->data_type = data_type;
    new_parameter->next = NULL;
    new_parameter->index = item->return_types_count++;

    if (item->return_types_list_first == NULL) {
        item->return_types_list_first = new_parameter;
    } else {
        method_param_structT *current = item->return_types_list_first;

        while (current->next != NULL) {
            current = current->next;
        }
        current->next = new_parameter;
    }
}

void late_check_stack_item_add_parameter(late_check_stack_item *item, Data_type data_type) {
    if (item == NULL) {
        return;
    }

    /* if allocation fails we exit the program */
    method_param_structT *new_parameter;
    if ((new_parameter = (method_param_structT *) malloc(sizeof(method_param_structT))) == NULL) {
        exit(SEMANTIC_LATE_CHECK_MALLOC_ERROR);
    }

    /* copies data type to new late check stack item parameter */
    new_parameter->data_type = data_type;
    new_parameter->next = NULL;
    new_parameter->index = item->parameters_count++;

    if (item->parameters_list_first == NULL) {
        /* if list is empty we can put it directly at list's first pointer */
        item->parameters_list_first = new_parameter;
    } else {
        /* else we need to chain new parameter at the end of the list  */
        method_param_structT *current = item->parameters_list_first;

        while (current->next != NULL) {
            current = current->next;
        }
        current->next = new_parameter;
    }
}

void late_check_stack_item_add_return_type(late_check_stack_item *item, Data_type data_type) {
    if (item == NULL) {
        return;
    }

    /* if allocation failed we exit the program  */
    method_param_structT *new_parameter;
    if ((new_parameter = (method_param_structT *) malloc(sizeof(method_param_structT))) == NULL) {
        exit(SEMANTIC_LATE_CHECK_MALLOC_ERROR);
    }
    new_parameter->data_type = data_type;
    new_parameter->next = NULL;
    new_parameter->index = item->return_types_count++;

    if (item->return_types_list_first == NULL) {
        /* if list is empty we can put it directly at list's first pointer */
        item->return_types_list_first = new_parameter;
    } else {
        /* else we need to chain new return type at the end of the list  */
        method_param_structT *current = item->return_types_list_first;

        while (current->next != NULL) {
            current = current->next;
        }
        current->next = new_parameter;
    }
}

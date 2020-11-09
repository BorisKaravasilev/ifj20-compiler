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
        free(tmp);
        s->size--;
    }
}

void late_check_stack_free(late_check_stack *s){
    late_check_stack_item *temp = NULL;

    while (s->top != NULL) {
        temp = s->top;
        s->top = s->top->next;
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

void late_check_stack_item_add_parameter(late_check_stack_item *item, Data_type data_type) {
    if (item == NULL) {
        return;
    }

    method_param_structT *new_parameter;
    if ((new_parameter = (method_param_structT *) malloc(sizeof(method_param_structT))) == NULL) {
        exit(SEMANTIC_LATE_CHECK_MALLOC_ERROR);
    }
    new_parameter->data_type = data_type;
    new_parameter->next = NULL;
    new_parameter->index = item->parameters_count++;

    if (item->parameters_list_first == NULL) {
        item->parameters_list_first = new_parameter;
    } else {
        method_param_structT *current = item->parameters_list_first;

        while (current->next != NULL) {
            current = current->next;
        }
        current->next = new_parameter;
    }
}



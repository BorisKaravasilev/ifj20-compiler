/**
 * Project: Implementation of a compiler of the IFJ20 language.
 *
 * @file semantic_functions.c
 * @brief Implementation of semantic checking functions.
 * @author Petr Vrtal <xvrtal01@stud.fit.vutbr.cz>
 * @date 9. 11. 2020
 */

#include "semantic_functions.h"

void assignment_struct_init(assignmentT *s) {
    if (s != NULL){
        assignmentT *tmp = (assignmentT *) malloc(sizeof(assignmentT));
        if (tmp != NULL){
            tmp->left_side_types_list_first = NULL;
            tmp->right_side_types_list_first = NULL;
        } else {
            // TODO: add stderr print
            exit(RC_RUN_ERR);
        }
    }
}

void assignment_struct_empty(assignmentT *s) {
    if (s != NULL) {
        s->identifiers_count = 0;
        s->expressions_count = 0;
        assignment_param_list_free(s->left_side_types_list_first);
        assignment_param_list_free(s->right_side_types_list_first);
        s->left_side_types_list_first = NULL;
        s->right_side_types_list_first = NULL;
    }
}

void assignment_struct_free(assignmentT *s) {
    if (s != NULL) {
        assignment_param_list_free(s->left_side_types_list_first);
        assignment_param_list_free(s->right_side_types_list_first);
        free(s);
    }
}

void assignment_param_list_free(assignment_paramT *assignment_param) {
    if (assignment_param != NULL) {
        assignment_paramT *temp;
        assignment_paramT *current = assignment_param;

        while (current != NULL) {
            temp = current;
            current = current->next;
            free(temp);
        }
    }
}

bool compare_left_right_params(assignment_paramT *left, assignment_paramT *right) {

}

void assignment_add_identifier(assignmentT *item, stringT *key){

}

void assignment_add_expression(assignmentT *item, Data_type type) {

}

/**
 * Project: Implementation of a compiler of the IFJ20 language.
 *
 * @file semantic_functions.c
 * @brief Implementation of semantic checking functions.
 * @author Petr Vrtal <xvrtal01@stud.fit.vutbr.cz>
 * @date 9. 11. 2020
 */

#include <token_functions.h>
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

void compare_left_right_params(assignment_paramT *left, assignment_paramT *right) {
    assignment_paramT *l_current = left;
    assignment_paramT *r_current = right;

    while (l_current != NULL || r_current != NULL) {
        if (l_current == NULL || r_current == NULL) {
            // TODO: Maybe another semantic error dont know atm
            exit(RC_SEMANTIC_OTHER_ERR);
        } else {
            if (l_current->data_type != r_current->data_type) {
                // TODO: Maybe another semantic error dont know atm
                exit(RC_SEMANTIC_OTHER_ERR);
            }
        }

        l_current = l_current->next;
        r_current = r_current->next;
    }
}

void assignment_add_identifier(assignmentT *item, int token_type, ST_Item *st_item){
    if (item == NULL || st_item == NULL) {
        return;
    }

    Data_type type = TYPE_ERROR;
    if (token_type == TOKEN_UNDERSCORE) {
        type = TYPE_BLANK_VARIABLE;
    } else {
        type = st_item->type;
    }

    assignment_paramT *new_id;
    if ((new_id = (assignment_paramT *) malloc(sizeof(assignment_paramT))) == NULL) {
        exit(RC_RUN_ERR);
    }
    new_id->data_type = type;
    new_id->next = NULL;
    new_id->index = item->identifiers_count++;

    if (item->left_side_types_list_first == NULL) {
        item->left_side_types_list_first = new_id;
    } else {
        assignment_paramT *current = item->left_side_types_list_first;

        while (current->next != NULL) {
            current = current->next;
        }
        current->next = new_id;
    }
}

void assignment_add_expression(assignmentT *item, Data_type type) {
    if (item == NULL) {
        return;
    }

    assignment_paramT *new_expr;
    if ((new_expr = (assignment_paramT *) malloc(sizeof(assignment_paramT))) == NULL) {
        exit(RC_RUN_ERR);
    }
    new_expr->data_type = type;
    new_expr->next = NULL;
    new_expr->index = item->expressions_count++;

    if (item->right_side_types_list_first == NULL) {
        item->right_side_types_list_first = new_expr;
    } else {
        assignment_paramT *current = item->right_side_types_list_first;

        while (current->next != NULL) {
            current = current->next;
        }
        current->next = new_expr;
    }
}

void assignment_add_user_function(assignmentT *item, ST_Item *function) {
    if (item == NULL || function == NULL) {
        return;
    }

    st_function_data_param_structT *current_function_return_type = function->function_data->return_types_list_first;
    while (current_function_return_type != NULL) {
        assignment_paramT *new_expr;
        if ((new_expr = (assignment_paramT *) malloc(sizeof(assignment_paramT))) == NULL) {
            exit(RC_RUN_ERR);
        }
        new_expr->data_type = current_function_return_type->data_type;
        new_expr->next = NULL;
        new_expr->index = item->expressions_count++;

        if (item->right_side_types_list_first == NULL) {
            item->right_side_types_list_first = new_expr;
        } else {
            assignment_paramT *current = item->right_side_types_list_first;

            while (current->next != NULL) {
                current = current->next;
            }
            current->next = new_expr;
        }

        current_function_return_type = current_function_return_type->next;
    }
}

void assignment_add_built_in_function(assignmentT *item, built_in_functionT *function) {
    if (item == NULL || function == NULL) {
        return;
    }

    for(int i = 0; i < function->return_types_count; i++) {
        Data_type return_type;
        switch (function->return_types[i]) {
            case 'i':
                return_type = TYPE_INT;
                break;
            case 'f':
                return_type = TYPE_DECIMAL;
                break;
            case 's':
                return_type = TYPE_STRING;
                break;
            default:
                break;
        }

        assignment_paramT *new_expr;
        if ((new_expr = (assignment_paramT *) malloc(sizeof(assignment_paramT))) == NULL) {
            exit(RC_RUN_ERR);
        }
        new_expr->data_type = return_type;
        new_expr->next = NULL;
        new_expr->index = item->expressions_count++;

        if (item->right_side_types_list_first == NULL) {
            item->right_side_types_list_first = new_expr;
        } else {
            assignment_paramT *current = item->right_side_types_list_first;

            while (current->next != NULL) {
                current = current->next;
            }
            current->next = new_expr;
        }
    }
}

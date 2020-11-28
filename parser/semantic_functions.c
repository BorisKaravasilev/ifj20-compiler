/**
 * Project: Implementation of a compiler of the IFJ20 language.
 *
 * @file semantic_functions.c
 * @brief Implementation of semantic checking functions.
 * @author Petr Vrtal <xvrtal01@stud.fit.vutbr.cz>
 * @date 9. 11. 2020
 */

#include "semantic_functions.h"

#include "token_functions.h"

void assignment_struct_init(assignmentT *s) {
    if (s != NULL) {
        assignmentT *tmp = (assignmentT *)malloc(sizeof(assignmentT));
        if (tmp != NULL) {
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

int compare_left_right_params(assignment_paramT *left, assignment_paramT *right) {
    assignment_paramT *l_current = left;
    assignment_paramT *r_current = right;

    while (l_current != NULL || r_current != NULL) {
        if (l_current == NULL || r_current == NULL) {
            // TODO: Maybe another semantic error dont know atm
            return RC_SEMANTIC_TYPE_COMPATIBILITY_ERR;
        } else {
            if (l_current->data_type != r_current->data_type) {
                // TODO: Maybe another semantic error dont know atm
                if (l_current->data_type != TYPE_BLANK_VARIABLE) {
                    return RC_SEMANTIC_TYPE_COMPATIBILITY_ERR;
                }
            }
        }

        l_current = l_current->next;
        r_current = r_current->next;
    }

    return SEMANTIC_OK;
}

int assignment_derive_id_type(assignmentT *s) {
    if (s->expressions_count != s->identifiers_count) {
        return RC_SEMANTIC_VAR_TYPE_ERR;
    }

    assignment_paramT *l_current = s->left_side_types_list_first;
    assignment_paramT *r_current = s->right_side_types_list_first;

    while (l_current != NULL) {
        if (l_current->data_type == TYPE_NIL) {
            Data_type right_side_data_type;
            if (r_current->symbol != NULL) {
                right_side_data_type = r_current->symbol->type;
            } else {
                right_side_data_type = r_current->data_type;
            }

            l_current->symbol->type = right_side_data_type;
            l_current->data_type = right_side_data_type;
        }

        l_current = l_current->next;
        r_current = r_current->next;
    }
    return SEMANTIC_OK;
}

void assignment_add_identifier(assignmentT *item, int token_type, ST_Item *st_item){
    if (item == NULL) {
        return;
    }

    assignment_paramT *new_id;
    if ((new_id = (assignment_paramT *)malloc(sizeof(assignment_paramT))) ==
        NULL) {
        exit(RC_RUN_ERR);
    }

    if (token_type == TOKEN_UNDERSCORE) {
        new_id->symbol = NULL;
        new_id->data_type = TYPE_BLANK_VARIABLE;
    } else {
        new_id->symbol = st_item;
        new_id->data_type = st_item->type;
    }

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

void assignment_add_expression(assignmentT *item, Data_type type,
                               ST_Item *symbol) {
    if (item == NULL) {
        return;
    }

    assignment_paramT *new_expr;
    if ((new_expr = (assignment_paramT *)malloc(sizeof(assignment_paramT))) ==
        NULL) {
        exit(RC_RUN_ERR);
    }

    if (symbol == NULL) {
        new_expr->symbol = NULL;
        new_expr->data_type = type;
    } else {
        new_expr->symbol = symbol;
        new_expr->data_type = symbol->type;
    }
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

    st_function_data_param_structT *current_function_return_type =
        function->function_data->return_types_list_first;
    while (current_function_return_type != NULL) {
        assignment_paramT *new_expr;
        if ((new_expr = (assignment_paramT *)malloc(
                 sizeof(assignment_paramT))) == NULL) {
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

// TODO: Do a propper check for passed builtin func parameters
built_in_functionT get_built_in_function_by_key(int token_type) {
    for (int i = 0; i < BUILT_IN_FUNCTIONS_COUNT; i++) {
        if (token_type == built_in_functions[i].function_token_type) {
            return built_in_functions[i];
        }
    }
}

Data_type get_data_type_from_char(char ch) {
    switch (ch) {
        case 'i':
            return TYPE_INT;
            break;
        case 'f':
            return TYPE_DECIMAL;
            break;
        case 's':
            return TYPE_STRING;
            break;
        default:
            return TYPE_ERROR;
            break;
    }
}

void assignment_add_built_in_function(assignmentT *item, int function_token) {
    if (item == NULL) {
        return;
    }

    built_in_functionT function = get_built_in_function_by_key(function_token);

    for(int i = 0; i < function.return_types_count; i++) {
        Data_type return_type = get_data_type_from_char(function.return_types[i]);

        assignment_paramT *new_expr;
        if ((new_expr = (assignment_paramT *)malloc(
                 sizeof(assignment_paramT))) == NULL) {
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

// TODO: Complete
int check_builtin_func_param_type_at_index(built_in_functionT *builtin_func, int index, Data_type type) {

    return SEMANTIC_OK;
}

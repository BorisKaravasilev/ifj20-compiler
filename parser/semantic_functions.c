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
            /* if allocated then initialize values to NULL  */
            tmp->left_side_types_list_first = NULL;
            tmp->right_side_types_list_first = NULL;
        } else {
            /* if allocation failed we exit the program with error code 99  */
            exit(RC_RUN_ERR);
        }
    }
}

void assignment_struct_empty(assignmentT *s) {
    if (s != NULL) {
        /* Empties all assignmentT struct data and its pointers  */
        s->function_call = false;
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

int compare_left_right_params(assignmentT *item) {
    assignment_paramT *l_current = item->left_side_types_list_first;
    assignment_paramT *r_current = item->right_side_types_list_first;

    /* cycle through all items in list  */
    while (l_current != NULL || r_current != NULL) {
        if (l_current == NULL || r_current == NULL) {
            /* if left or right param type is NULL it indicates different types of parameters  */
            if (item->function_call) {
                /* if we were assigning a function we specified bad return types for it  */
                return RC_SEMANTIC_FUNC_PARAM_ERR;
            } else {
                return RC_SEMANTIC_OTHER_ERR;
            }
        } else {
            if (l_current->data_type != r_current->data_type) {
                /* if data types for both parameters differs it may be an semantic error  */
                if (l_current->data_type != TYPE_BLANK_VARIABLE) {
                    /* we need to check if param at the left of the assignment is underscore or not  */
                    if (item->function_call) {
                        return RC_SEMANTIC_FUNC_PARAM_ERR;
                    } else {
                        return RC_SEMANTIC_OTHER_ERR;
                    }
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
        /* if parameter count differ between the left and right params count its an type derivation semantic error */
        return RC_SEMANTIC_VAR_TYPE_ERR;
    }

    assignment_paramT *l_current = s->left_side_types_list_first;
    assignment_paramT *r_current = s->right_side_types_list_first;

    /* cycles through all identifiers in the left side types list */
    while (l_current != NULL) {
        if (l_current->data_type == TYPE_NIL) {
            /* if identifier type is still its value from initialization  */
            Data_type right_side_data_type;
            if (r_current->symbol != NULL) {
                /* if param on the right is identifier we add its symtable data type  */
                right_side_data_type = r_current->symbol->type;
            } else {
                /* if not we just simply add its type  */
                right_side_data_type = r_current->data_type;
            }

            /* Derive identifier data type  */
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

    /* if allocation failed we exit the program */
    assignment_paramT *new_id;
    if ((new_id = (assignment_paramT *)malloc(sizeof(assignment_paramT))) ==
        NULL) {
        exit(RC_RUN_ERR);
    }


    if (token_type == TOKEN_UNDERSCORE) {
        new_id->symbol = NULL;
        new_id->data_type = TYPE_BLANK_VARIABLE;
    } else {
        /* if given param is identifier we copy its type from symtable */
        new_id->symbol = st_item;
        new_id->data_type = st_item->type;
    }

    new_id->next = NULL;
    new_id->index = item->identifiers_count++; /* increments assignment structure identifiers count  */

    if (item->left_side_types_list_first == NULL) {
        /* if list is empty we can put it directly at list's first pointer */
        item->left_side_types_list_first = new_id;
    } else {
        /* else we need to chain new parameter at the end of the list  */
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

    /* if allocation failed we exit the program */
    assignment_paramT *new_expr;
    if ((new_expr = (assignment_paramT *)malloc(sizeof(assignment_paramT))) ==
        NULL) {
        exit(RC_RUN_ERR);
    }

    if (symbol == NULL) {
        new_expr->symbol = NULL;
        new_expr->data_type = type;
    } else {
        /* if given param is identifier we copy its type from symtable */
        new_expr->symbol = symbol;
        new_expr->data_type = symbol->type;
    }

    new_expr->next = NULL;
    new_expr->index = item->expressions_count++;

    if (item->right_side_types_list_first == NULL) {
        /* if list is empty we can put it directly at list's first pointer */
        item->right_side_types_list_first = new_expr;
    } else {
        assignment_paramT *current = item->right_side_types_list_first;

        /* else we need to chain new parameter at the end of the list  */
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

    item->function_call = true;
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

built_in_functionT get_built_in_function_by_key(int token_type) {
    /* find associated builtin function by its token type and return it */
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
    item->function_call = true;
    built_in_functionT function = get_built_in_function_by_key(function_token);

    /* Copy all builtin function return types into assignment struct param list  */
    for(int i = 0; i < function.return_types_count; i++) {
        Data_type return_type = get_data_type_from_char(function.return_types[i]);

        /* if allocation failed we exit the program  */
        assignment_paramT *new_expr;
        if ((new_expr = (assignment_paramT *)malloc(
                 sizeof(assignment_paramT))) == NULL) {
            exit(RC_RUN_ERR);
        }

        /* copy data to new parameter */
        new_expr->data_type = return_type;
        new_expr->next = NULL;
        new_expr->index = item->expressions_count++;

        if (item->right_side_types_list_first == NULL) {
            /* if list is empty we can put it directly at list's first pointer */
            item->right_side_types_list_first = new_expr;
        } else {
            /* else we need to chain new parameter at the end of the list  */
            assignment_paramT *current = item->right_side_types_list_first;

            while (current->next != NULL) {
                current = current->next;
            }
            current->next = new_expr;
        }
    }
}

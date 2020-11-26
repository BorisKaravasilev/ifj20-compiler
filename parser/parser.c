/**
 * Project: Implementation of a compiler of the IFJ20 language.
 *
 * @file parser.c
 * @brief Parser implementation
 * @author Dominik Večeřa <xvecer23@stud.fit.vutbr.cz>
 * @author Petr Vrtal <xvrtal01@stud.fit.vutbr.cz>
 */

#include "parser.h"

int token_index = 0, tmp_result = 0;
bool was_expr = false, unget_token = false;

assignmentT assignment_check_struct;
ST_Item *current_function_block_symbol = NULL;

late_check_stack semantic_late_check_stack;

/*
    puts("-----------------------------------");
    for (int i = 0; i < TOKEN_ARRAY_LEN; i++)
        printf("%d: %d\n", i, token[i].token_type);
    puts("-----------------------------------");
 */

/*
 * Print error string to stderr
 */
void err_print(char* str, int token_type){
    fprintf(stderr, "Expected %s but got token %d.\n", str, token_type);
}

/*
 * An identifier - regular or special _
 */
int id(scannerT *ptr_scanner, tokenT token[]){
    if (!unget_token) {
        get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // identifier or _
    }
    else {
        unget_token = false;
    }

    if (token[token_index].token_type == TOKEN_UNDERSCORE) {
        return SYNTAX_OK;
    }
    else if (token[token_index].token_type == TOKEN_IDENTIFIER) {
        // TODO prostor na semantiku :D
        return SYNTAX_OK;
    }

    return RC_SYN_ERR;
}

/*
 * An item - integer, float or string
 */
int literal(scannerT *ptr_scanner, tokenT token[], int *item_type){ // TODO P is item_type also necessary here?
    if (!unget_token) {
        get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // int, float or string literal
    }
    else {
        unget_token = false;
    }


    if (token[token_index].token_type == TOKEN_INTEGER_LITERAL ||
        token[token_index].token_type == TOKEN_DECIMAL_LITERAL ||
        token[token_index].token_type == TOKEN_EXPONENT_LITERAL ||
        token[token_index].token_type == TOKEN_STRING_LITERAL) {

        if (item_type != NULL) {    // TODO P semantic actions change or remove?
            switch(token[token_index].token_type) {
                case TOKEN_INTEGER_LITERAL:
                    *item_type = TYPE_INT;
                    break;
                case TOKEN_DECIMAL_LITERAL:
                case TOKEN_EXPONENT_LITERAL:
                    *item_type = TYPE_DECIMAL;
                    break;
                case TOKEN_STRING_LITERAL:
                    *item_type = TYPE_STRING;
                    break;
                default:
                    break;
            }
        }
        return SYNTAX_OK;
    }

    return RC_SYN_ERR;
}

/*
 * An item - integer, float or string
 */
int item(scannerT *ptr_scanner, tokenT token[], int *item_type){
    if (!unget_token) {
        get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // int, float or string keyword
    }
    else {
        unget_token = false;
    }

    if (token[token_index].token_type == TOKEN_KEYWORD_INT ||
        token[token_index].token_type == TOKEN_KEYWORD_FLOAT64 ||
        token[token_index].token_type == TOKEN_KEYWORD_STRING) {

        if (item_type != NULL) {
            switch(token[token_index].token_type) {
                case TOKEN_KEYWORD_INT:
                    *item_type = TYPE_INT;
                    break;
                case TOKEN_KEYWORD_FLOAT64:
                    *item_type = TYPE_DECIMAL;
                    break;
                case TOKEN_KEYWORD_STRING:
                    *item_type = TYPE_STRING;
                    break;
                default:
                    break;
            }
        }
        return SYNTAX_OK;
    }

    return RC_SYN_ERR;
}

/*
 * The optional continuation of a print statement
 */
int print_next(scannerT *ptr_scanner, tokenT token[]){
    get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // , or )

    switch (token[token_index].token_type) {
        case TOKEN_RIGHT_BRACKET:
            return SYNTAX_OK;

        case TOKEN_COMMA:
            get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // int / float / str literal, id

            if (token[token_index].token_type == TOKEN_IDENTIFIER){
                return print_next(ptr_scanner, token);
            }
            else {
                unget_token = true;
                tmp_result = literal(ptr_scanner, token, NULL); // TODO P item_type?
                if (tmp_result != SYNTAX_OK)
                    return tmp_result;

                return print_next(ptr_scanner, token);
            }

        default:
            err_print(", or )", token[token_index].token_type);
            return RC_SYN_ERR;
    }
}

/*
 * The beginning of a print statement
 */
int print(scannerT *ptr_scanner, tokenT token[]){
    get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // int / float / str literal, id, )

    switch (token[token_index].token_type) {
        case TOKEN_RIGHT_BRACKET:
            return SYNTAX_OK;

        case TOKEN_IDENTIFIER:
            return print_next(ptr_scanner, token);

        default:
            unget_token = true;
            tmp_result = literal(ptr_scanner, token, NULL); // TODO P item_type?
            if (tmp_result != SYNTAX_OK)
                return tmp_result;

            return print_next(ptr_scanner, token);
    }
}

/*
 * A built-in function
 */
int builtin_func(scannerT *ptr_scanner, tokenT token[], int *built_in_func_type){
    if (!unget_token) {
        get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // built-in function name
    }
    else {
        unget_token = false;
    }

    switch (token[token_index].token_type) {
        case TOKEN_FUNCTION_INPUTS:
        case TOKEN_FUNCTION_INPUTI:
        case TOKEN_FUNCTION_INPUTF:
            if (built_in_func_type != NULL) {
                *built_in_func_type = token[token_index].token_type;
            }
            get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // (

            if (token[token_index].token_type != TOKEN_LEFT_BRACKET){
                err_print("(", token[token_index].token_type);
                return RC_SYN_ERR;
            }

            get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // )

            if (token[token_index].token_type == TOKEN_RIGHT_BRACKET){
                return SYNTAX_OK;
            }
            else {
                err_print(")", token[token_index].token_type);
                return RC_SYN_ERR;
            }

        case TOKEN_FUNCTION_PRINT:
            if (built_in_func_type != NULL) {
                *built_in_func_type = token[token_index].token_type;
            }
            get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // (

            if (token[token_index].token_type != TOKEN_LEFT_BRACKET){
                err_print("(", token[token_index].token_type);
                return RC_SYN_ERR;
            }

            return print(ptr_scanner, token);

        case TOKEN_FUNCTION_INT2FLOAT: // TODO P review split int2float and chr
            if (built_in_func_type != NULL) {
                *built_in_func_type = token[token_index].token_type;
            }
            get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // (

            if (token[token_index].token_type != TOKEN_LEFT_BRACKET){
                err_print("(", token[token_index].token_type);
                return RC_SYN_ERR;
            }

            get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // int

            if (token[token_index].token_type != TOKEN_INTEGER_LITERAL){
                err_print("int", token[token_index].token_type);
                return RC_SYN_ERR;
            }

            get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // )

            if (token[token_index].token_type == TOKEN_RIGHT_BRACKET){
                return SYNTAX_OK;
            }
            else {
                err_print(")", token[token_index].token_type);
                return RC_SYN_ERR;
            }

        case TOKEN_FUNCTION_FLOAT2INT:
            if (built_in_func_type != NULL) {
                *built_in_func_type = token[token_index].token_type;
            }
            get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // (

            if (token[token_index].token_type != TOKEN_LEFT_BRACKET){
                err_print("(", token[token_index].token_type);
                return RC_SYN_ERR;
            }

            get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // float

            if (token[token_index].token_type != TOKEN_DECIMAL_LITERAL ||
                token[token_index].token_type != TOKEN_EXPONENT_LITERAL){
                err_print("float", token[token_index].token_type);
                return RC_SYN_ERR;
            }

            get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // )

            if (token[token_index].token_type == TOKEN_RIGHT_BRACKET){
                return SYNTAX_OK;
            }
            else {
                err_print(")", token[token_index].token_type);
                return RC_SYN_ERR;
            }

        case TOKEN_FUNCTION_LEN:
            if (built_in_func_type != NULL) {
                *built_in_func_type = token[token_index].token_type;
            }
            get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // (

            if (token[token_index].token_type != TOKEN_LEFT_BRACKET){
                err_print("(", token[token_index].token_type);
                return RC_SYN_ERR;
            }

            get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // string

            if (token[token_index].token_type != TOKEN_STRING_LITERAL){
                err_print("string", token[token_index].token_type);
                return RC_SYN_ERR;
            }

            get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // )

            if (token[token_index].token_type == TOKEN_RIGHT_BRACKET){
                return SYNTAX_OK;
            }
            else {
                err_print(")", token[token_index].token_type);
                return RC_SYN_ERR;
            }

        case TOKEN_FUNCTION_SUBSTR:
            if (built_in_func_type != NULL) {
                *built_in_func_type = token[token_index].token_type;
            }
            get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // (

            if (token[token_index].token_type != TOKEN_LEFT_BRACKET){
                err_print("(", token[token_index].token_type);
                return RC_SYN_ERR;
            }

            get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // string

            if (token[token_index].token_type != TOKEN_STRING_LITERAL){
                err_print("string", token[token_index].token_type);
                return RC_SYN_ERR;
            }

            get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // ,

            if (token[token_index].token_type != TOKEN_COMMA){
                err_print(",", token[token_index].token_type);
                return RC_SYN_ERR;
            }

            get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // int

            if (token[token_index].token_type != TOKEN_INTEGER_LITERAL){
                err_print("int", token[token_index].token_type);
                return RC_SYN_ERR;
            }

            get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // ,

            if (token[token_index].token_type != TOKEN_COMMA){
                err_print(",", token[token_index].token_type);
                return RC_SYN_ERR;
            }

            get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // int

            if (token[token_index].token_type != TOKEN_INTEGER_LITERAL){
                err_print("int", token[token_index].token_type);
                return RC_SYN_ERR;
            }

            get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // )

            if (token[token_index].token_type == TOKEN_RIGHT_BRACKET){
                return SYNTAX_OK;
            }
            else {
                err_print(")", token[token_index].token_type);
                return RC_SYN_ERR;
            }

        case TOKEN_FUNCTION_ORD:
            if (built_in_func_type != NULL) {
                *built_in_func_type = token[token_index].token_type;
            }
            get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // (

            if (token[token_index].token_type != TOKEN_LEFT_BRACKET){
                err_print("(", token[token_index].token_type);
                return RC_SYN_ERR;
            }

            get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // string

            if (token[token_index].token_type != TOKEN_STRING_LITERAL){
                err_print("string", token[token_index].token_type);
                return RC_SYN_ERR;
            }

            get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // ,

            if (token[token_index].token_type != TOKEN_COMMA){
                err_print(",", token[token_index].token_type);
                return RC_SYN_ERR;
            }

            get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // int

            if (token[token_index].token_type != TOKEN_INTEGER_LITERAL){
                err_print("int", token[token_index].token_type);
                return RC_SYN_ERR;
            }

            get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // )

            if (token[token_index].token_type == TOKEN_RIGHT_BRACKET){
                return SYNTAX_OK;
            }
            else {
                err_print(")", token[token_index].token_type);
                return RC_SYN_ERR;
            }

        case TOKEN_FUNCTION_CHR: // TODO P review split int2float and chr
            if (built_in_func_type != NULL) {
                *built_in_func_type = token[token_index].token_type;
            }
            get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // (

            if (token[token_index].token_type != TOKEN_LEFT_BRACKET){
                err_print("(", token[token_index].token_type);
                return RC_SYN_ERR;
            }

            get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // int

            if (token[token_index].token_type != TOKEN_INTEGER_LITERAL){
                err_print("int", token[token_index].token_type);
                return RC_SYN_ERR;
            }

            get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // )

            if (token[token_index].token_type == TOKEN_RIGHT_BRACKET){
                return SYNTAX_OK;
            }
            else {
                err_print(")", token[token_index].token_type);
                return RC_SYN_ERR;
            }

        default:
            return RC_SYN_ERR;
    }
}

/*
 * An expression - resolved by precedential syntactic analysis
 */
int expr(scannerT *ptr_scanner, tokenT token[], bool two_tokens, int *result_data_type){
    if (!unget_token) {
        get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // beginning of expr
    }
    else {
        unget_token = false;
    }

    tokenT last_expr_token, expr_result_token;
    token_init(&last_expr_token);
    token_init(&expr_result_token);

    puts("\nEntering expression analysis\n");   // TODO D remove
    if (two_tokens){
        tmp_result = expr_check(&token[token_index - 1], &token[token_index], &last_expr_token, &expr_result_token, ptr_scanner);
        printf("\nExited expression analysis with %d.\n", tmp_result);   // TODO D remove
        if (tmp_result != SYNTAX_OK)
            return tmp_result;
    }
    else {
        tmp_result = expr_check(NULL, &token[token_index], &last_expr_token, &expr_result_token, ptr_scanner);
        printf("\nExited expression analysis with %d.\n", tmp_result);   // TODO D remove
        if (tmp_result != SYNTAX_OK)
            return tmp_result;
    }

    // SEMANTIC
    if (result_data_type != NULL) {
        switch (expr_result_token.token_type) {
            case EXPRESSION_INT:
                *result_data_type = TYPE_INT;
                break;
            case EXPRESSION_FLOAT64:
                *result_data_type = TYPE_DECIMAL;
                break;
            case EXPRESSION_STRING:
                *result_data_type = TYPE_STRING;
                break;
            default:
                break;
        }
    }
    // SEMANTIC END

    return SYNTAX_OK;
}

/*
 * An individual assignment (without function calls)
 */
int assign_nofunc(scannerT *ptr_scanner, tokenT token[]){
    if (!unget_token) {
        get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // id, item or expression
    }
    else {
        unget_token = false;
    }

    unget_token = true;

    int item_type;
    tmp_result = literal(ptr_scanner, token, &item_type); // TODO P parameter?
    if (tmp_result == SYNTAX_OK) {
        // TODO P this was after calling <item> before, semantic change?
        assignment_add_expression(&assignment_check_struct, item_type, NULL);
        return SYNTAX_OK;
    }

    unget_token = true;

    if (token[token_index].token_type == TOKEN_IDENTIFIER){
        ST_Item *identifier;
        if ((identifier = stack_search(&ptr_scanner->st_stack, &token[token_index].attribute.string_val)) == NULL) {
            fprintf(stderr, "Error: Undefined variable \'%s\'", token[token_index].attribute.string_val.string);
            return RC_SEMANTIC_IDENTIFIER_ERR;
        }
        // TODO: add id on right side to assign struct
        // TODO P can this be removed, since it's the same as the following code, or do you need to add something here?
        /*int expr_result_type;
        int result = expr(ptr_scanner, token, false, &expr_result_type);
        was_expr = true;
        assignment_add_expression(&assignment_check_struct, expr_result_type, NULL);
        return result;*/
    }

    int expr_result_type;
    int result = expr(ptr_scanner, token, false, &expr_result_type);
    was_expr = true;
    assignment_add_expression(&assignment_check_struct, expr_result_type, NULL);
    return result;
}

/*
 * The optional continuation of a list of assignments (without function calls)
 */
int assign_nofunc_next(scannerT *ptr_scanner, tokenT token[]){
    if (!unget_token) {
        get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // , or extra token was read
    }
    else {
        unget_token = false;
    }

    if (token[token_index].token_type == TOKEN_COMMA){
        tmp_result = assign_nofunc(ptr_scanner, token);
        if (tmp_result != SYNTAX_OK)
            return tmp_result;

        return assign_nofunc_next(ptr_scanner, token);
    }
    else {
        unget_token = true;
        return SYNTAX_OK;
    }
}

/*
 * The beginning of a list of assignments (without function calls)
 */
int assign_nofunc_list(scannerT *ptr_scanner, tokenT token[]){
    tmp_result = assign_nofunc(ptr_scanner, token);
    if (tmp_result != SYNTAX_OK)
        return tmp_result;

    return assign_nofunc_next(ptr_scanner, token);
}

/*
 * An individual assignment
 */
int assign(scannerT *ptr_scanner, tokenT token[], bool *skip_sides_semantic_type_check){
    if (!unget_token) {
        get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // id, item, built-in function or expression
    }
    else {
        unget_token = false;
    }

    unget_token = true;

    int item_type;
    tmp_result = literal(ptr_scanner, token, &item_type); // TODO P parameter?
    if (tmp_result == SYNTAX_OK) {
        // TODO P this was after calling <item> before, semantic change?
        assignment_add_expression(&assignment_check_struct, item_type, NULL);
        return SYNTAX_OK;
    }

    unget_token = true;

    int built_in_func_type_token;
    tmp_result = builtin_func(ptr_scanner, token, &built_in_func_type_token);
    if (tmp_result == SYNTAX_OK) {
        assignment_add_built_in_function(&assignment_check_struct, built_in_func_type_token);
        return SYNTAX_OK;
    }

    if (token[token_index].token_type == TOKEN_IDENTIFIER){
        // check following token, if (, then function parameter list follows, otherwise an extra token was read
        get_next_token(ptr_scanner, &token[++token_index], OPTIONAL);

        if (token[token_index].token_type == TOKEN_LEFT_BRACKET) {
            // TODO FIRST: add function call to late check struct
            if (skip_sides_semantic_type_check != NULL) {
                *skip_sides_semantic_type_check = true;
            }
            late_check_stack_push_method(&semantic_late_check_stack, &token[token_index-1].attribute.string_val);
            late_check_stack_item_create_assignment_list(semantic_late_check_stack.top, assignment_check_struct.left_side_types_list_first);
            return id_list1(ptr_scanner, token);
        }
        else {
            ST_Item *identifier;
            if ((identifier = stack_search(&ptr_scanner->st_stack, &token[token_index-1].attribute.string_val)) == NULL) {
                fprintf(stderr, "Error: Undefined variable \'%s\'", token[token_index-1].attribute.string_val.string);
                return RC_SEMANTIC_IDENTIFIER_ERR;
            }
            int expr_result_type;
            int result = expr(ptr_scanner, token, true, &expr_result_type);
            was_expr = true;
            assignment_add_expression(&assignment_check_struct, expr_result_type, identifier);
            return result;
        }
    }

    int expr_result_type;
    int result = expr(ptr_scanner, token, false, &expr_result_type);
    was_expr = true;
    assignment_add_expression(&assignment_check_struct, expr_result_type, NULL);
    return result;
}

/*
 * The optional continuation of a list of assignments
 */
int assign_next(scannerT *ptr_scanner, tokenT token[]){
    if (!unget_token) {
        get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // , or extra token was read
    }
    else {
        unget_token = false;
    }

    if (token[token_index].token_type == TOKEN_COMMA){
        tmp_result = assign(ptr_scanner, token, NULL);
        if (tmp_result != SYNTAX_OK)
            return tmp_result;

        return assign_next(ptr_scanner, token);
    }
    else {
        unget_token = true;
        return SYNTAX_OK;
    }
}

/*
 * The beginning of a list of assignments
 */
int assign_list(scannerT *ptr_scanner, tokenT token[]){
    tmp_result = assign(ptr_scanner, token, NULL);
    if (tmp_result != SYNTAX_OK)
        return tmp_result;

    return assign_next(ptr_scanner, token);
}

/*
 * The optional continuation of a list of identifiers
 */
int id_next2(scannerT *ptr_scanner, tokenT token[]){
    get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // = or ,

    if (token[token_index].token_type == TOKEN_EQUAL) {
        return SYNTAX_OK;
    }
    else if (token[token_index].token_type == TOKEN_COMMA){
        tmp_result = id(ptr_scanner, token);
        if (tmp_result == SYNTAX_OK) {
            return id_next2(ptr_scanner, token);
        }
        else {
            err_print("id or _", token[token_index].token_type);
            return RC_SYN_ERR;
        }
    }
    else {
        err_print(", or =", token[token_index].token_type);
        return RC_SYN_ERR;
    }
}

/*
 * The beginning of a list of identifiers
 */
int id_list2(scannerT *ptr_scanner, tokenT token[]){
    get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // id, _ or =

    if (token[token_index].token_type == TOKEN_EQUAL) {
        return SYNTAX_OK;
    }
    else {
        unget_token = true; // id or _ is already loaded
        tmp_result = id(ptr_scanner, token);
        if (tmp_result == SYNTAX_OK) {
            return id_next2(ptr_scanner, token);
        }
        else {
            err_print("id, _ or =", token[token_index].token_type);
            return RC_SYN_ERR;
        }
    }
}

/*
 * The optional continuation of a list of identifiers
 */
int id_next1(scannerT *ptr_scanner, tokenT token[]){
    get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // ) or ,
    ST_Item *identifier = NULL;
    switch (token[token_index].token_type){
        case TOKEN_RIGHT_BRACKET:
            return SYNTAX_OK;

        case TOKEN_COMMA:
            get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); //  id

            if (token[token_index].token_type != TOKEN_IDENTIFIER){
                err_print("id", token[token_index].token_type);
                return RC_SYN_ERR;
            }

            if ((identifier = stack_search(&ptr_scanner->st_stack, &token[token_index].attribute.string_val)) == NULL) {
                fprintf(stderr, "Error: Undefined variable \'%s\'", token[token_index].attribute.string_val.string);
                return RC_SEMANTIC_IDENTIFIER_ERR;
            }
            late_check_stack_item_add_parameter(semantic_late_check_stack.top, identifier->type);

            return id_next1(ptr_scanner, token);

        default:
            err_print(") or ,", token[token_index].token_type);
            return RC_SYN_ERR;
    }
}

/*
 * The beginning of a list of identifiers
 */
int id_list1(scannerT *ptr_scanner, tokenT token[]){
    get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // id or )
    ST_Item *identifier = NULL;

    switch (token[token_index].token_type){
        case TOKEN_RIGHT_BRACKET:
            return SYNTAX_OK;

        case TOKEN_IDENTIFIER:
            if ((identifier = stack_search(&ptr_scanner->st_stack, &token[token_index].attribute.string_val)) == NULL) {
                fprintf(stderr, "Error: Undefined variable \'%s\'", token[token_index].attribute.string_val.string);
                return RC_SEMANTIC_IDENTIFIER_ERR;
            }
            late_check_stack_item_add_parameter(semantic_late_check_stack.top, identifier->type);
            return id_next1(ptr_scanner, token);

        default:
            err_print("id or )", token[token_index].token_type);
            return RC_SYN_ERR;
    }
}

/*
 * A command with an identifier (definition, one or multiple assignments, function call)
 */
int id_command(scannerT *ptr_scanner, tokenT token[]){
    Symtable *ptr_curr_scope_sym_table = stack_top(&ptr_scanner->st_stack).symtable;
    ST_Item *symbol = NULL;
    int result;
    bool skip_semantic_check = false;

    printf("Entering id_command, current token: %d\n", token[token_index].token_type); // TODO D rm

    get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // :=, =, ( or ,

    switch (token[token_index].token_type){
        case TOKEN_COLON_EQUAL:
            // SEMANTIC: Insert identifier to symtable
            assignment_struct_empty(&assignment_check_struct);
            symbol = st_insert_symbol(ptr_curr_scope_sym_table, &token[token_index-1].attribute.string_val, false);
            assignment_add_identifier(&assignment_check_struct, token[token_index-1].token_type, symbol);
            // END SEMANTIC
            result = assign_nofunc(ptr_scanner, token);
            if (was_expr){
                unget_token = true;
                was_expr = false;
            }
            if (result == SYNTAX_OK) {
                assignment_derive_id_type(&assignment_check_struct);
            }
            return result;
        case TOKEN_EQUAL:
            // TODO: Change to another non terminal because of cycle assign
            // SEMANTIC: Check if identifier exists in symtable
            // TODO: Proper check for "_" identifier
            if (string_compare_constant(&token[token_index-1].attribute.string_val, "_") != 0) {
                if ((symbol = stack_search(&ptr_scanner->st_stack, &token[token_index-1].attribute.string_val)) == NULL) {
                    fprintf(stderr, "Error: Undefined variable \'%s\'", token[token_index-1].attribute.string_val.string);
                    return RC_SEMANTIC_IDENTIFIER_ERR;
                }
            }
            assignment_struct_empty(&assignment_check_struct);
            assignment_add_identifier(&assignment_check_struct, token[token_index-1].token_type, symbol);
            // END SEMANTIC

            result = assign(ptr_scanner, token, &skip_semantic_check);
            if (result == SYNTAX_OK && !skip_semantic_check) {
                return compare_left_right_params(assignment_check_struct.left_side_types_list_first, assignment_check_struct.right_side_types_list_first);
            }
            return result;

        case TOKEN_LEFT_BRACKET:
            // TODO: Begin function call late check by creating function in that structure
            // TODO FIRST: add function call to late check struct
            late_check_stack_push_method(&semantic_late_check_stack, &token[token_index-1].attribute.string_val);
            return id_list1(ptr_scanner, token);

        case TOKEN_COMMA:
            // TODO: multiple left side ids assignment
            tmp_result = id_list2(ptr_scanner, token);
            if (tmp_result != SYNTAX_OK)
                return tmp_result;

            return assign_list(ptr_scanner, token);

        default:
            err_print(":=, =, ( or ,", token[token_index].token_type);
            return RC_SYN_ERR;
    }
}

/*
 * An optional assignment at the end of each cycle iteration
 */
int cycle_assign(scannerT *ptr_scanner, tokenT token[]){
    get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // id, _ or {

    if (token[token_index].token_type == TOKEN_LEFT_CURLY_BRACE){
        return SYNTAX_OK;
    }
    else {
        unget_token = true; // id or _ is already loaded
        tmp_result = id(ptr_scanner, token);
        if (tmp_result == SYNTAX_OK){
            get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // =

            if (token[token_index].token_type != TOKEN_EQUAL){
                err_print("=", token[token_index].token_type);
                return RC_SYN_ERR;
            }

            tmp_result = expr(ptr_scanner, token, false, NULL);
            if (tmp_result != SYNTAX_OK)
                return tmp_result;

            if (token[token_index].token_type != TOKEN_LEFT_CURLY_BRACE){
                err_print("{", token[token_index].token_type);
                return RC_SYN_ERR;
            }

            return SYNTAX_OK;
        }
        else {
            err_print("id, _ or {", token[token_index].token_type);
            return RC_SYN_ERR;
        }
    }
}

/*
 * Optional initial definition in a cycle
 */
int cycle_init(scannerT *ptr_scanner, tokenT token[]){
    get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // id or ;

    if (token[token_index].token_type == TOKEN_SEMICOLON){
        return SYNTAX_OK;
    }
    else if (token[token_index].token_type == TOKEN_IDENTIFIER){
        get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // :=

        if (token[token_index].token_type != TOKEN_COLON_EQUAL){
            err_print(":=", token[token_index].token_type);
            return RC_SYN_ERR;
        }

        tmp_result = assign_nofunc(ptr_scanner, token);
        if (tmp_result != SYNTAX_OK)
            return tmp_result;

        if (!unget_token) {
            get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // ;
        }
        else {
            unget_token = false;
        }

        if (token[token_index].token_type == TOKEN_SEMICOLON){
            return SYNTAX_OK;
        }
        else {
            err_print(";", token[token_index].token_type);
            return RC_SYN_ERR;
        }
    }
    else {
        err_print("id or ;", token[token_index].token_type);
        return RC_SYN_ERR;
    }
}

/*
 * An individual command inside a function, cycle, or block
 */
int command(scannerT *ptr_scanner, tokenT token[]){
    // probably shouldn't get token here, since parser already has it from cmd_list
    printf("\n\nEntering command w/ token type %d.\n\n", token[token_index].token_type); // TODO D rm

    switch (token[token_index].token_type){
        case TOKEN_IDENTIFIER:
            return id_command(ptr_scanner, token);

        case TOKEN_KEYWORD_IF:
            tmp_result = expr(ptr_scanner, token, false, NULL);
            if (tmp_result != SYNTAX_OK)
                return tmp_result;

            if (token[token_index].token_type != TOKEN_LEFT_CURLY_BRACE){
                err_print("{", token[token_index].token_type);
                return RC_SYN_ERR;
            }

            tmp_result = command_list(ptr_scanner, token);
            if (tmp_result != SYNTAX_OK)
                return tmp_result;

            if (!unget_token) {
                get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // else
            }
            else {
                unget_token = false;
            }

            if (token[token_index].token_type != TOKEN_KEYWORD_ELSE){
                err_print("else keyword", token[token_index].token_type);
                return RC_SYN_ERR;
            }

            get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // {

            if (token[token_index].token_type != TOKEN_LEFT_CURLY_BRACE){
                err_print("{", token[token_index].token_type);
                return RC_SYN_ERR;
            }

            return command_list(ptr_scanner, token);

        case TOKEN_KEYWORD_FOR:
            tmp_result = cycle_init(ptr_scanner, token);
            if (tmp_result != SYNTAX_OK)
                return tmp_result;

            tmp_result = expr(ptr_scanner, token, false, NULL);
            if (tmp_result != SYNTAX_OK)
                return tmp_result;

            if (token[token_index].token_type != TOKEN_SEMICOLON){
                err_print(";", token[token_index].token_type);
                return RC_SYN_ERR;
            }

            tmp_result = cycle_assign(ptr_scanner, token);
            if (tmp_result != SYNTAX_OK)
                return tmp_result;

            return command_list(ptr_scanner, token);

        case TOKEN_KEYWORD_RETURN:
            // a function can return zero or more values
            get_next_token(ptr_scanner, &token[++token_index], OPTIONAL);  // } or assignment(s)

            unget_token = true;
            if (token[token_index].token_type == TOKEN_RIGHT_CURLY_BRACE){
                return SYNTAX_OK;
            }
            else {
                return assign_nofunc_list(ptr_scanner, token);
            }

        default:
            break;
    }

    unget_token = true;
    tmp_result = builtin_func(ptr_scanner, token, NULL);
    if (tmp_result == SYNTAX_OK)
        return SYNTAX_OK;

    return RC_SYN_ERR;
}

/*
 * The list of commands inside a function, cycle, or block
 */
int command_list(scannerT *ptr_scanner, tokenT token[]){
    if (!unget_token) {
        get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // command or }
    }
    else {
        unget_token = false;
    }

    printf("\n\nEntering command_list w/ token type %d.\n\n", token[token_index].token_type); // TODO D rm

    if (token[token_index].token_type == TOKEN_RIGHT_CURLY_BRACE){
        return SYNTAX_OK;
    }
    else {
        tmp_result = command(ptr_scanner, token);
        if (tmp_result != SYNTAX_OK)
            return tmp_result;

        return command_list(ptr_scanner, token);
    }
}

/*
 * The optional continuation of a list of function return types
 */
int return_type(scannerT *ptr_scanner, tokenT token[], ST_Item *ptr_curr_symbol){
    get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // , or )

    if (token[token_index].token_type == TOKEN_RIGHT_BRACKET){
        get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // {

        if (token[token_index].token_type == TOKEN_LEFT_CURLY_BRACE){
            return SYNTAX_OK;
        }
        else {
            err_print("{", token[token_index].token_type);
            return RC_SYN_ERR;
        }
    }
    else if (token[token_index].token_type == TOKEN_COMMA){
        int item_type;
        tmp_result = item(ptr_scanner, token, &item_type);
        if (tmp_result != SYNTAX_OK) {
            return tmp_result;
        }
        st_add_function_return_type(ptr_curr_symbol, item_type);

        return return_type(ptr_scanner, token, ptr_curr_symbol);
    }
    else {
        err_print(", or }", token[token_index].token_type);
        return RC_SYN_ERR;
    }
}

/*
 * The beginning of a list of function return types
 */
int return_type_list(scannerT *ptr_scanner, tokenT token[], ST_Item *ptr_curr_symbol){
    get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // ( or {

    if (token[token_index].token_type == TOKEN_LEFT_CURLY_BRACE){
        return SYNTAX_OK;
    }
    else if (token[token_index].token_type != TOKEN_LEFT_BRACKET){
        err_print("( or {", token[token_index].token_type);
        return RC_SYN_ERR;
    }

    get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // ) or item

    if (token[token_index].token_type == TOKEN_RIGHT_BRACKET){
        get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // {

        if (token[token_index].token_type == TOKEN_LEFT_CURLY_BRACE){
            return SYNTAX_OK;
        }
        else {
            err_print("{", token[token_index].token_type);
            return RC_SYN_ERR;
        }
    }
    else {  // item received
        unget_token = true;
        int item_type;
        tmp_result = item(ptr_scanner, token, &item_type);
        if (tmp_result != SYNTAX_OK) {
            return tmp_result;
        }
        st_add_function_return_type(ptr_curr_symbol, item_type);

        return return_type(ptr_scanner, token, ptr_curr_symbol);
    }
}

/*
 * The optional continuation of a list of function parameters
 */
int param(scannerT *ptr_scanner, tokenT token[], ST_Item *ptr_curr_symbol){
    get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // , or )

    if (token[token_index].token_type == TOKEN_RIGHT_BRACKET){
        return SYNTAX_OK;
    }
    else if (token[token_index].token_type != TOKEN_COMMA){
        err_print(", or )", token[token_index].token_type);
        return RC_SYN_ERR;
    }

    get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // function parameter (ID)

    if (token[token_index].token_type == TOKEN_IDENTIFIER){
        int item_type;
        tmp_result = item(ptr_scanner, token, &item_type);
        if (tmp_result != SYNTAX_OK) {
            return tmp_result;
        }
        st_add_function_param(ptr_curr_symbol, item_type);
    }

    return param(ptr_scanner, token, ptr_curr_symbol);
}

/*
 * The beginning of a list of function parameters
 */
int param_list(scannerT *ptr_scanner, tokenT token[], ST_Item *ptr_curr_symbol){
    get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // function parameter (ID) or )

    if (token[token_index].token_type == TOKEN_RIGHT_BRACKET){
        return SYNTAX_OK;
    }
    else if (token[token_index].token_type == TOKEN_IDENTIFIER){
        int item_type;
        Symtable *ptr_curr_scope_sym_table = stack_top(&ptr_scanner->st_stack).symtable;
        ST_Item *newParamSymbol = st_insert_symbol(ptr_curr_scope_sym_table, &token[token_index].attribute.string_val, false);

        tmp_result = item(ptr_scanner, token, &item_type);
        if (tmp_result != SYNTAX_OK) {
            return tmp_result;
        }
        st_add_function_param(ptr_curr_symbol, item_type);
        newParamSymbol->type = item_type;
    }
    else {
        err_print("function parameter (ID) or )", token[token_index].token_type);
        return RC_SYN_ERR;
    }

    return param(ptr_scanner, token, ptr_curr_symbol);
}

/*
 * A function definition
 */
int func(scannerT *ptr_scanner, tokenT token[]){
    get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // function name (ID)

    if (token[token_index].token_type != TOKEN_IDENTIFIER){
        err_print("identifier (function name)", token[token_index].token_type);
        return RC_SYN_ERR;
    }

    // SEMANTIC: Add function to symtable
    Symtable *ptr_curr_scope_sym_table = stack_top(&ptr_scanner->st_stack).symtable;
    ST_Item *symbol = st_insert_symbol(ptr_curr_scope_sym_table, &token[token_index].attribute.string_val, true);
    current_function_block_symbol = symbol;
    // END SEMANTIC

    get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // left opening bracket

    if (token[token_index].token_type != TOKEN_LEFT_BRACKET){
        err_print("left opening bracket", token[token_index].token_type);
        return RC_SYN_ERR;
    }

    tmp_result = param_list(ptr_scanner, token, symbol);
    if (tmp_result != SYNTAX_OK)
        return tmp_result;

    tmp_result = return_type_list(ptr_scanner, token, symbol);
    if (tmp_result != SYNTAX_OK)
        return tmp_result;

    tmp_result = command_list(ptr_scanner, token);
    if (tmp_result != SYNTAX_OK)
        return tmp_result;

    return SYNTAX_OK;
}

/*
 * The program itself - a list of functions
 */
int program(scannerT *ptr_scanner, tokenT token[]){
    get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // "func" / EOF

    if (token[token_index].token_type == TOKEN_KEYWORD_FUNC){
        tmp_result = func(ptr_scanner, token);
        if (tmp_result != SYNTAX_OK)
            return tmp_result;

        return program(ptr_scanner, token);
    }
    else if (token[token_index].token_type == TOKEN_EOF){
        return SYNTAX_OK;
    }

    err_print("func keyword or EOL", token[token_index].token_type);
    return RC_SYN_ERR;
}

/*
 * Starting point of parser
 */
int parse(scannerT *ptr_scanner, tokenT token[]){
    late_check_stack_init(&semantic_late_check_stack);

    get_next_token(ptr_scanner, &token[token_index], OPTIONAL); // "package"

    if (token[token_index].token_type != TOKEN_KEYWORD_PACKAGE){
        err_print("package keyword", token[token_index].token_type);
        return RC_SYN_ERR;
    }

    get_next_token(ptr_scanner, &token[++token_index], OPTIONAL); // "main"

    if (token[token_index].token_type != TOKEN_IDENTIFIER ||
    !string_compare_constant(&token[token_index].attribute.string_val, "main\n")){ // TODO remove newline
        err_print("main keyword", token[token_index].token_type);
        return RC_SYN_ERR;
    }

    return program(ptr_scanner, token);
}

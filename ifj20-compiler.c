/**
 * Project: Implementation of a compiler of the IFJ20 language.
 * @file    ifj20-compiler.c
 * @brief   Main file that brings all the parts together.
 * @author  Boris Karavasilev <xkarav01@stud.fit.vutbr.cz>
 * @date    3. 10. 2020
 */

#include <stdio.h>
#include <stdbool.h>

#include "scanner_functions.h"
#include "parser.h"
#include "debugging.h"
#include "generator.h"

int main(int argc, char** argv) {
    // For debugging purposes in CLion (input file or stdin)
    FILE *input_fp = get_input_file(IS_INPUT_FROM_FILE, argv[1]);

    scannerT scanner;
    init_scanner(&scanner, input_fp);

    tokenT token[TOKEN_ARRAY_LEN];
    token_array_init(token, TOKEN_ARRAY_LEN);

    stack_init(&scanner.st_stack);
    stack_push(&scanner.st_stack);

    // Top level symbol table
    Symtable *global_scope_symtable = stack_top(&scanner.st_stack).symtable;

    int return_code = parse(&scanner, token);

    if (return_code != 0)
    {
        print_error_message_by_code(return_code, &scanner.file_pos);
        return return_code;
    }

    // Semantic check if main function is present
    stringT main_function_string;
    string_init(&main_function_string);
    string_add_string(&main_function_string, "main");
    ST_Item *main_func_symbol = st_search(global_scope_symtable, &main_function_string);
    if (main_func_symbol == NULL) {
        fprintf(stderr, "Missing function \'main\'.");
        return RC_SEMANTIC_IDENTIFIER_ERR;
    }
    if (main_func_symbol->function_data->parameters_list_first != NULL || main_func_symbol->function_data->return_types_list_first != NULL) {
        fprintf(stderr, "Function \'main\' must not have any parameters or return types.");
        return RC_SEMANTIC_FUNC_PARAM_ERR;
    }

    // FREE ALL ALLOCATED MEMORY
    token_array_free(token, TOKEN_ARRAY_LEN);
    free_scanner(&scanner);
    string_free(&main_function_string);

    return 0;
}

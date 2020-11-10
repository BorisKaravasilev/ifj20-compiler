/**
 * Project: Implementation of a compiler of the IFJ20 language.
 * @file    ifj20-compiler.c
 * @brief   Main file that brings all the parts together.
 * @author  Boris Karavasilev <xkarav01@stud.fit.vutbr.cz>
 * @date    3. 10. 2020
 */

#include <stdio.h>
#include <stdbool.h>

#include "scanner/finite_automata.h"
#include "scanner/scanner_functions.h"
#include "general/utility_functions.h"
#include "scanner/token_types.h"
#include "general/return_codes.h"
#include "scanner/symtable.h"
#include "scanner/token_functions.h"
#include "scanner/stack.h"
#include "parser/semantic_late_check.h"

#define IS_INPUT_FROM_FILE true // Input from file or stdin

int main(int argc, char** argv) {
    // For debugging purposes in CLion (input file or stdin)
    FILE *input_fp = get_input_file(IS_INPUT_FROM_FILE, argv[1]);

    scannerT scanner;
    init_scanner(&scanner, input_fp);

    // TODO: Change to dynamically allocated array or remember necessary tokens in parser
    tokenT token[TOKEN_ARRAY_LEN];
    token_array_init(token, TOKEN_ARRAY_LEN);

    Symtable *symtable = st_init();
    Stack stack;
    stack_init(&stack);
    stack_push(&stack, symtable);

    stringT testString;
    string_init(&testString);
    string_add_string(&testString, "testMethod");

    late_check_stack late_check;
    late_check_stack_init(&late_check);

    late_check_stack_push_method(&late_check, &testString);
    late_check_stack_item_add_parameter(late_check_stack_search(&late_check, &testString), TYPE_DECIMAL);
    late_check_stack_item_add_parameter(late_check_stack_search(&late_check, &testString), TYPE_STRING);
    late_check_stack_item_add_return_type(late_check_stack_search(&late_check, &testString), TYPE_INT);

    printf("late_check_method_found_name: %s, params: [type: %d]\n", late_check_stack_search(&late_check, &testString)->method_name.string);

    late_check_stack_free(&late_check);
    clear_str(&testString);

    for (int i = 0; i < TOKEN_ARRAY_LEN; i++) {
        get_next_token(&scanner, &token[i], OPTIONAL);

        debug_token(&token[i], i);

        if (token[i].token_type == TOKEN_EOF) break;
    }

    // FREE ALL ALLOCATED MEMORY
    token_array_free(token, TOKEN_ARRAY_LEN);
    free_scanner(&scanner);

    return 0;
}

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
#define INPUT_FILE_NAME "input_files/input_1.txt"

int main() {
    // For debugging purposes in CLion (input file or stdin)
    FILE *input_fp = get_input_file(IS_INPUT_FROM_FILE, INPUT_FILE_NAME);

    // Modify 'finite_automata.c' to change FA graph
    finite_automataT fa;
    init_finite_automata(&fa);

    // TODO: Change single token that is being cleaned and overwritten
    // to dynamically allocated array of tokens that grows over time
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
    late_check_stack_item_add_parameter(late_check_stack_search(&late_check, &testString), &testString, TYPE_DECIMAL);

    printf("late_check_method_found_name: %s, params: [id: %s]\n", late_check_stack_search(&late_check, &testString)->method_name.string, late_check_stack_search(&late_check, &testString)->parameters_list_first->name.string);

    late_check_stack_free(&late_check);
    clear_str(&testString);

    for (int i = 0; i < TOKEN_ARRAY_LEN; i++) {
        get_next_token(&fa, input_fp, &stack, &token[i]);
        debug_token(&token[i], i);

        if (token[i].token_type == TOKEN_EOF) break;
    }

    // Just for testing (all the inner blocks are popped, so only globally defined identifiers will be found)
    stringT test_string;
    string_init(&test_string);
    string_add_string(&test_string, "num1");

    ST_Item *found_item = stack_search(&stack, &test_string);

    if (found_item != NULL) {
        printf("\nFound symtable result with key '%s'\n", found_item->key.string);
    } else {
        printf("\nNot found in symtable\n");
    } // end of stack symtable testing code

    // FREE ALL ALLOCATED MEMORY
    token_array_free(token, TOKEN_ARRAY_LEN);
    stack_free(&stack);

    return 0;
}

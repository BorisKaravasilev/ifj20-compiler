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

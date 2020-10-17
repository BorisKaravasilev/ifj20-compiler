// Boris Karavasilev <xkarav01@stud.fit.vutbr.cz>
// TODO: Standardize file header

#include <stdio.h>
#include <stdbool.h>

#include "scanner/finite_automata.h"
#include "scanner/scanner_functions.h"
#include "general/utility_functions.h"
#include "scanner/token_types.h"
#include "general/return_codes.h"
#include "scanner/symtable.h"
#include "scanner/token_functions.h"

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

    symtableT symtable; // TODO: Make array, add hash function, add stack
    symtable_init(&symtable);


    for (int i = 0; i < TOKEN_ARRAY_LEN; i++) {
        get_next_token(&fa, input_fp, &symtable, &token[i]);
        debug_token(&token[i], i);

        if (token[i].token_type == TOKEN_EOF) break;
    }

    debug_whole_symtable(&symtable);

    // FREE ALL ALLOCATED MEMORY
    token_array_free(token, TOKEN_ARRAY_LEN);
    symtable_free(&symtable);

    return 0;
}

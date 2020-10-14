// Boris Karavasilev <xkarav01@stud.fit.vutbr.cz>

#include <stdio.h>
#include <stdbool.h>

#include "scanner/finite_automata.h"
#include "scanner/scanner_functions.h"
#include "general/utility_functions.h"
#include "scanner/token_types.h"
#include "general/return_codes.h"
#include "scanner/symtable.h"

#define IS_INPUT_FROM_FILE true // Input from file or stdin
#define INPUT_FILE_NAME "input_files/input_1.txt"

// TODO: Clean and refactor
int main() {
    // For debugging purposes in CLion (input file or stdin)
    FILE *input_fp = get_input_file(IS_INPUT_FROM_FILE, INPUT_FILE_NAME);

    // Modify 'finite_automata.c' to change FA graph
    finite_automataT fa;
    init_finite_automata(&fa);

    // TODO: Change single token that is being cleaned and overwritten
    // to dynamically allocated array of tokens that grows over time
    tokenT token[50];

    // Init all tokens
    for (int i = 0; i < 50; i++) {
        token_init(&token[i]);
    }

    symtableT symtable;
    symtable_init(&symtable);

    int counter = 0;

    do {
        counter++;
        // TODO: pass EOL flag to get_next_token()
        get_next_token(&fa, input_fp, &symtable, &token[counter - 1]);
        printf("[--> Received token type: '%i', attribute: '%s']\n", token[counter - 1].token_type, token[counter - 1].attribute.string_val.string);

        // Deciding which final state the processing ended at
        if (token[counter - 1].token_type == TOKEN_AB) {
            printf("TOKEN #%d | type %d | 'a' or 'b' letters\n", counter,
                   token[counter - 1].token_type);
        } else if (token[counter - 1].token_type == TOKEN_10) {
            printf("TOKEN #%d | type %d | '0' or '1' digits\n", counter, token[counter - 1].token_type);
        }
        // TODO: Keywords implementation inside syntactic analyzer
        //else if (token.token_type == TOKEN_KEYWORD_STRING) {
        //    printf("TOKEN #%d | type %d | keyword string\n", counter, token.token_type);
        //}
    } while (!(token[counter - 1].token_type == TOKEN_ERR || token[counter - 1].token_type == TOKEN_EOF));

    // Display whole symbol table
    for (int i = 0; i < symtable.length; ++i) {
        printf("FINAL SYMTABLE item[%d]: \"%s\"\n", i, symtable.ptr_item[i].ptr_string);
    }

    // FREE ALL TOKEN INTERNALS
    // Init all tokens
    for (int i = 0; i < 50; i++) {
        token_free(&token[i]);
    }
    // FREE SYMBOLTABLE
    symtable_free(&symtable);

    // TODO: Error fix -> place it inside token_generation
    if (token[counter - 1].token_type == TOKEN_ERR) {
        return RC_LEX_ERR;
    }

    return 0;
}
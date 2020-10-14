// Boris Karavasilev <xkarav01@stud.fit.vutbr.cz>

#include <stdio.h>
#include <stdbool.h>

#include "scanner/finite_automata.h"
#include "scanner/scanner_functions.h"
#include "general/utility_functions.h"
#include "scanner/token_types.h"
#include "general/return_codes.h"

#define IS_INPUT_FROM_FILE true // Input from file or stdin
#define INPUT_FILE_NAME "input_files/input_1.txt"

int main() {
    // For debugging purposes in CLion (input file or stdin)
    FILE *input_fp = get_input_file(IS_INPUT_FROM_FILE, INPUT_FILE_NAME);

    // Modify 'finite_automata_definition.c' to change FA graph
    finite_automataT fa;
    init_finite_automata(&fa);

    //int token = 0;
    tokenT token;
    token_init(&token);

    int counter = 0;

    do {
        counter++;
        // TODO: pass EOL flag to get_next_token()
        get_next_token(&fa, input_fp, &token);
        printf("[--> Received token type: '%i', attribute: '%s']\n", token.token_type, token.attribute.string_val.string);

        // Deciding which final state the processing ended at
        if (token.token_type == TOKEN_AB) {
            printf("TOKEN #%d | type %d | 'a' or 'b' letters\n", counter,
                   token.token_type);
        } else if (token.token_type == TOKEN_10) {
            printf("TOKEN #%d | type %d | '0' or '1' digits\n", counter, token.token_type);
        }
        // TODO: Keywords implementation inside syntactic analyzer
        //else if (token.token_type == TOKEN_KEYWORD_STRING) {
        //    printf("TOKEN #%d | type %d | keyword string\n", counter, token.token_type);
        //}
    } while (!(token.token_type == TOKEN_ERR || token.token_type == TOKEN_EOF));

    // TODO: Error fix -> place it inside token_generation
    if (token.token_type == TOKEN_ERR) {
        return RC_LEX_ERR;
    }

    return 0;
}
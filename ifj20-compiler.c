// Boris Karavasilev <xkarav01@stud.fit.vutbr.cz>

#include <stdio.h>
#include <stdbool.h>

#include "scanner/finite_automata_definition.h"
#include "scanner/lex_functions.h"
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

    int token = 0;
    int counter = 0;

    do {
        counter++;
        token = get_next_token(&fa, input_fp);

        // Deciding which final state the processing ended at
        if (token == TOKEN_AB) {
            printf("TOKEN #%d | type %d | 'a' or 'b' letters\n", counter,
                   token);
        } else if (token == TOKEN_10) {
            printf("TOKEN #%d | type %d | '0' or '1' digits\n", counter, token);
        }
    } while (!(token == TOKEN_ERR || token == TOKEN_EOF));

    if (token == TOKEN_ERR) {
        return RC_LEX_ERR;
    }

    return 0;
}
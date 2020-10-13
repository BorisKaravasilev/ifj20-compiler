#include <stdbool.h>
#include <stdio.h>

#include "lex_functions.h"
#include "token_types.h"
#include "token_functions.h"

#define ERROR_NO_NEXT_STATE -2

// Global variables

// TODO: Move skip symbols in different file
// Symbols that should be ignored (eg., blank spaces, line endings ...)
char skip_symbols[] = {' ', '\n'};
int skip_symbols_len = sizeof(skip_symbols) / sizeof(char);

int get_next_state(char curr_sym, int curr_state, finite_automataT *fa) {
    // Go through all rules in FA
    for (int i = 0; i < RULES_LEN; i++) {
        // Is rule initialized or empty? (unused array cell check)
        if (fa->rules[i].from_state >= START_STATE) {
            // Rule for current state?
            if (fa->rules[i].from_state == curr_state) {
                // Go through all transition symbols in rule
                // (symbols that bring you to next state)
                for (int j = 0; j < TRANS_SYM_LEN; j++) {
                    // TODO: Change to is_in_range()
                    if (fa->rules[i].transition_symbols[j] == curr_sym) {
                        int next_state = fa->rules[i].to_state;
                        curr_state = next_state;
                        return next_state;
                    }
                }
            }
        }
    }

    // Can't transition to any state with curr_sym
    return ERROR_NO_NEXT_STATE;
}

//bool is_in_range() {
    // is 'ragne_or_char.from == -1'
    // -> then compare singe_char to current char
    // else
    // if (from <= curr && curr <= to)
    // -> return true
//}

bool is_final_state(int state, finite_automataT *fa) {
    for (int i = 0; i < FINAL_STATES_LEN; i++) {
        if (fa->final_states[i] == state) {
            return true;
        }
    }

    return false;
}

// TODO: Actual token generation and symbol tabel to be done
token_struct generate_token(token_struct *token, int type) {
    token->token_type = keyword_check(token, type);
    return *token;
}

// TODO: Replace magical number return codes with defined constants
token_struct get_next_token(finite_automataT *fa, FILE *input_file, token_struct *token) {
    int curr_state = START_STATE;
    int next_state;
    static char curr_sym = 0;

    // Ensures reading of last character that belongs to next token
    static bool read_last_sym_from_previous_word = false;

    // Cleans the content of token_val string
    token_clear(token);

    while (curr_sym != EOF) {
        if (!read_last_sym_from_previous_word) {
            curr_sym = fgetc(input_file);

            if (curr_sym != EOF && curr_sym != ' ') {
                token_val_add_char(token, curr_sym);
            }

            printf("Reading char...\n");
        }

        read_last_sym_from_previous_word = false;

        printf("curr_sym: '%c' \n", curr_sym);
        next_state = get_next_state(curr_sym, curr_state, fa);

        if (next_state != ERROR_NO_NEXT_STATE) {
            curr_state = next_state;
        } else {
            read_last_sym_from_previous_word = true;

            // No rule was matched
            if (is_final_state(curr_state, fa)) {
                // returns token struct (represents lexical unit)
                return generate_token(token, curr_state);
            } else {
                if (curr_sym != EOF) {
                    // TODO: Guess closest lexical units for correction
                    // suggestion (continue until you reach some final states)
                    fprintf(stderr, "\nLexical error detected! Finished at symbol: '%c' \n", curr_sym);
                    return generate_token(token, TOKEN_ERR);  // Lexical error
                }
            }
        }
    }

    // End Of File -> finished successfully
    return generate_token(token, TOKEN_EOF);
}

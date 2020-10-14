#include <stdbool.h>
#include <stdio.h>

#include "scanner_functions.h"
#include "token_types.h"

#define ERROR_NO_NEXT_STATE -2

int get_next_state(char curr_sym, int curr_state, finite_automataT *ptr_fa) {
    // Go through all rules in FA
    for (int i = 0; i < RULES_LEN; i++) {
        // Is rule initialized or empty? (unused array cell check)
        if (ptr_fa->rules[i].from_state >= START_STATE) {
            // Rule for current state?
            if (ptr_fa->rules[i].from_state == curr_state) {
                // Go through all transition symbols in rule
                // (symbols that bring you to next state)
                for (int j = 0; j < TRANS_SYM_LEN; j++) {
                    // TODO: Change to is_in_range()
                    if (ptr_fa->rules[i].transition_symbols[j] == curr_sym) {
                        int next_state = ptr_fa->rules[i].to_state;
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

bool is_final_state(int state, finite_automataT *ptr_fa) {
    for (int i = 0; i < FINAL_STATES_LEN; i++) {
        if (ptr_fa->final_states[i] == state) {
            return true;
        }
    }

    return false;
}

// TODO: Actual token generation and symbol tabel to be done
void generate_token(tokenT *ptr_token, int type) {
    // TODO: check if identifier else return original type function attr
    ptr_token->token_type = keyword_check(ptr_token, type);
}

void get_next_token(finite_automataT *ptr_fa, FILE *input_file, tokenT *ptr_token) {
    int curr_state = START_STATE;
    int next_state;
    static char curr_sym = 0;

    // Ensures reading of last character that belongs to next token
    static bool read_last_sym_from_previous_word = false;

    // Cleans the content of token_val string
    token_clear(ptr_token);

    while (curr_sym != EOF) {
        if (!read_last_sym_from_previous_word) {
            curr_sym = fgetc(input_file);

            printf("Reading char...\n");
        }

        read_last_sym_from_previous_word = false;

        printf("curr_sym: '%c' \n", curr_sym); // TODO: Make conditional debug prints with global macro
        next_state = get_next_state(curr_sym, curr_state, ptr_fa);

        if (next_state != ERROR_NO_NEXT_STATE) {
            curr_state = next_state;

            if (curr_sym != EOF && curr_sym != ' ') {
                token_val_add_char(ptr_token, curr_sym);
            }
        } else {
            read_last_sym_from_previous_word = true;

            // No rule was matched
            if (is_final_state(curr_state, ptr_fa)) {
                // returns token struct (represents lexical unit)
                generate_token(ptr_token, curr_state);
                return;
            } else {
                if (curr_sym != EOF) {
                    fprintf(stderr, "\nLexical error detected! Finished at symbol: '%c' \n", curr_sym);
                    generate_token(ptr_token, TOKEN_ERR);  // Lexical error
                    return;
                }
            }
        }
    }

    // End Of File -> finished successfully
    generate_token(ptr_token, TOKEN_EOF);
    return;
}

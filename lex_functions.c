#include "lex_functions.h"

#include <stdbool.h>
#include <stdio.h>

#define ERROR_NO_NEXT_STATE -2

// Global variables

// Symbols that should be ignored (eg., blank spaces, line endings ...)
char skip_symbols[] = {' ', '\n'};
int skip_symbols_len = sizeof(skip_symbols) / sizeof(char);

// TODO: Replace magical numbers with constants shared with 'lex_structures.h'
int rules_len = 50;
int trans_sym_len = 50;
int final_states_len = 50;

int get_next_state(char curr_sym, int curr_state, finite_automataT *fa) {
    // Go through all rules in FA
    for (int i = 0; i < rules_len; i++) {
        // Is rule initialized or empty? (unused array cell check)
        if (fa->rules[i].from_state >= 1) {
            // Rule for current state?
            if (fa->rules[i].from_state == curr_state) {
                // Go through all transition symbols in rule
                // (symbols that bring you to next state)
                for (int j = 0; j < trans_sym_len; j++) {
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

bool is_final_state(int state, finite_automataT *fa) {
    for (int i = 0; i < final_states_len; i++) {
        if (fa->final_states[i] == state) {
            return true;
        }
    }

    return false;
}

// TODO: Actual token generation and symbol tabel to be done
void generate_token() {}

// TODO: Replace magical number return codes with defined constants
int get_next_token(finite_automataT *fa) {
    int curr_state = 1;  // 1 is start state in graph
    int next_state;
    static char curr_sym = 0;

    // Ensures reading of last character that belongs to next token
    static bool read_last_sym_from_previous_word = false;

    while (curr_sym != EOF) {
        if (!read_last_sym_from_previous_word) {
            curr_sym = getchar();
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
                generate_token();
                // returns final state number (represents lexical unit)
                return curr_state;
            } else {
                if (curr_sym != EOF) {
                    // TODO: Guess closest lexical units for correction
                    // suggestion (continue until you reach some final states)
                    printf(
                        "\nLexical error detected! Finished at symbol: '%c' \n",
                        curr_sym);
                    return -1;  // Lexical error
                }
            }
        }
    }

    // End Of File -> finished successfully
    return -2;
}

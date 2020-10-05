#include "lex_functions.h"

#include <stdbool.h>
#include <stdio.h>

#define ERROR_NO_NEXT_STATE -1

// Global variables

// Symbols that should be ignored (eg., blank spaces, line endings ...)
char skip_symbols[] = {' ', '\n'};
int skip_symbols_len = sizeof(skip_symbols) / sizeof(char);

// finite_automataT fa;
int rules_len = 50;      // sizeof(fa.rules) / sizeof(ruleT);
int trans_sym_len = 50;  // sizeof(fa.rules.transition_symbols) / sizeof(char);
int final_states_len = 50;

// Returns next valid symbol from 'stdin'
char get_next_sym() {
    char next_sym;
    bool is_skip_sym;

    do {
        is_skip_sym = false;
        next_sym = getchar();

        // Check if current symbol is a skip symbol
        for (int i = 0; i < skip_symbols_len; i++) {
            if (skip_symbols[i] == next_sym) {
                is_skip_sym = true;
                break;
            }
        }

        if (!is_skip_sym) {
            return next_sym;
        }
    } while (next_sym != EOF);

    return EOF;
}

int get_next_state(char curr_sym, int curr_state, finite_automataT *fa) {
    // Go through all rules in FA
    for (int i = 0; i < rules_len; i++) {
        // printf("Rule %d from state %d\n", i, fa->rules[i].from_state);
        // Is rule initialized or empty?
        if (fa->rules[i].from_state >= 1) {
            // printf("INIT  from state: %d  curr_state: %d\n",
            // fa->rules[i].from_state, curr_state);
            // Rule for current state?
            if (fa->rules[i].from_state == curr_state) {
                // Go through all transition symbols in rule
                for (int j = 0; j < trans_sym_len; j++) {
                    if (fa->rules[i].transition_symbols[j] == curr_sym) {
                        int next_state = fa->rules[i].to_state;
                        // printf("Going from state %d to state %d\n",
                        // curr_state,
                        //     next_state);
                        // Go to next state according to rule
                        curr_state = next_state;
                        return next_state;
                    }
                }
            }
        }
    }

    return ERROR_NO_NEXT_STATE;  // Can't transition to any state with curr_sym
}

bool is_final_state(int state, finite_automataT *fa) {
    for (int i = 0; i < final_states_len; i++) {
        if (fa->final_states[i] == state) {
            return true;
        }
    }

    return false;
}

void generate_token() {}

int get_next_token(finite_automataT *fa) {
    int curr_state = 1;
    int next_state;
    static char curr_sym = 0;

    while (curr_sym != EOF) {
        curr_sym = getchar();
        // printf("curr_sym: '%c' -> %d \n", curr_sym, curr_sym);
        next_state = get_next_state(curr_sym, curr_state, fa);

        if (next_state != ERROR_NO_NEXT_STATE) {
            curr_state = next_state;
        } else {
            if (is_final_state(curr_state, fa)) {
                generate_token();
                // returns final state number (represents lexical unit)
                return curr_state;
            } else {
                if (curr_sym != EOF) {
                    // TODO: Guess closest lexical units for correction
                    // suggestion
                    printf("Lexical error detected! Current symbol: %c \n",
                           curr_sym);
                    return -1;
                }
            }
        }
    }

    return -2;
}

/*
get_next_token(char &character_after_token) {
    curr_state = start_state;

    while (not end of file) {
        curr_sym = get_character();

        if (can_transition_form_curr_state(curr_sym) == true) {
            curr_state = get_next_state();
        } else {
            if (if_final_state(curr_state) == true) {
                generate_token();
                return new_token;
            } else {
                guess_closest_lex_unit();
                log_error();
                return error_token; // End compilation
            }
        }
    }
}
*/

/*
// Symbols that should be ignored (eg., blank spaces, line endings ...)
char skip_symbols[] = {' ', '\n'};
int skip_symbols_len = sizeof(skip_symbols) / sizeof(char);

// Returns next valid symbol from 'stdin'
char get_next_sym() {
    char next_sym;
    bool is_skip_sym;

    do {
        is_skip_sym = false;
        next_sym = getchar();

        // Check if current symbol is a skip symbol
        for (int i = 0; i < skip_symbols_len; i++) {
            if (skip_symbols[i] == next_sym) {
                is_skip_sym = true;
                break;
            }
        }

        if (!is_skip_sym) {
            return next_sym;
        }
    } while (next_sym != EOF);

    return EOF;
}
*/
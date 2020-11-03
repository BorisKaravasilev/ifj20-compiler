#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "scanner_functions.h"
#include "token_types.h"
#include "states_list.h"
#include "../general/return_codes.h"
#include "../general/debugging.h"

#define ERROR_NO_NEXT_STATE -2

// Does a given symbol allow a transition in finite automaton?
bool is_accepted(char sym, range_or_charT *transition_ranges) {
    for (int i = 0; i < TRANS_RANGES_LEN; i++) { // go through all available slots
        if (transition_ranges[i].single_char != -1) { // single character
            if (transition_ranges[i].single_char == sym) { // compare character
                return true;
            }
        }
        else { // range
            if (transition_ranges[i].from <= sym && sym <= transition_ranges[i].to) {
                return true; // check if character is in accepted range
            }
        }
    }

    return false;
}

// Finds the next state of finite automaton
int get_next_state(char curr_sym, int curr_state, finite_automataT *ptr_fa) {
    if (curr_sym == EOF) return ERROR_NO_NEXT_STATE;

    // Go through all rules in FA
    for (int i = 0; i < RULES_LEN; i++) {
        ruleT *curr_rule = &ptr_fa->rules[i];
        int next_state = try_rule_transition(curr_sym, curr_state, curr_rule);

        if (next_state != ERROR_NO_NEXT_STATE) {
            debug_scanner("\nGoing from sate %d to state %d with rule #%d\n", curr_state, next_state, i);
            return next_state;
        }
    }

    // Can't transition to any state with curr_sym
    return ERROR_NO_NEXT_STATE;
}

// Tries to transition with symbol and rule from state
int try_rule_transition(char sym, int state, ruleT *rule) {
    // Is rule initialized? (unused array cell check)
    if (rule->from_state != TOKEN_EMPTY) {
        // Rule for current state?
        if (rule->from_state == state) {
            // Go through all transition symbols in rule
            // (symbols that bring you to next state)
            if (is_accepted(sym, rule->transition_ranges)) {
                int next_state = rule->to_state;
                return next_state;
            }
        }
    }

    return ERROR_NO_NEXT_STATE; // Can't transition with current rule
}

// Checks if a state is in the set of final states of finite automaton
bool is_final_state(int state, finite_automataT *ptr_fa) {
    for (int i = 0; i < FINAL_STATES_LEN; i++) {
        if (ptr_fa->final_states[i] == state) {
            return true;
        }
    }

    return false;
}

// TODO: Actual token generation and symbol label to be done
void generate_token(tokenT *ptr_token, Stack *ptr_stack, int type) {
    ptr_token->token_type = type;

    if (type == TOKEN_IDENTIFIER) {
        // Set token type to keyword, build-in function or identifier
        ptr_token->token_type = keyword_check(ptr_token);
        ptr_token->token_type = function_word_check(ptr_token);
    }

    if (ptr_token->token_type == TOKEN_IDENTIFIER) {
        stringT *key = &ptr_token->attribute.string_val;
        Symtable *ptr_curr_scope_sym_table = stack_top(ptr_stack).symtable;
        st_insert_symbol(ptr_curr_scope_sym_table, key, 0);
        debug_scanner("\n | SYMTABLE | [INSERTED KEY: '%s' TO SYMTABLE]\n\n\n", key->string);
    } else if (ptr_token->token_type == TOKEN_LEFT_CURLY_BRACE) {
        // Push new symtable block
        debug_scanner("\n | SYMTABLE | [PUSH NEW SYMTABLE BLOCK]\n\n\n%s", "");
        stack_push(ptr_stack, st_init());
    } else if (ptr_token->token_type == TOKEN_RIGHT_CURLY_BRACE) {
        // Pop symtable block
        debug_scanner("\n | SYMTABLE | [POP SYMTABLE BLOCK]\n\n\n%s", "");
        stack_pop(ptr_stack);
    }
}

// TODO: pass EOL flag to get_next_token()
void get_next_token(finite_automataT *ptr_fa, FILE *input_file, Stack *ptr_stack, tokenT *ptr_token) {
    int curr_state = STATE_START;
    int next_state;
    static char curr_sym = 0;
    static file_positionT file_pos = {1, 0};

    // Ensures reading of last character that belongs to next token
    static bool read_last_sym_from_previous_word = false;

    // Cleans the content of token_val string
    token_clear(ptr_token);

    while (curr_sym != EOF) {
        if (!read_last_sym_from_previous_word) {
            curr_sym = fgetc(input_file);
            update_file_position(&file_pos, curr_sym);
        }

        read_last_sym_from_previous_word = false;

        next_state = get_next_state(curr_sym, curr_state, ptr_fa);
        bool end_of_comment = is_end_of_comment(ptr_token, curr_state, next_state);
        bool is_string = curr_state == STATE_STRING || next_state == STATE_STRING;

        if (next_state != ERROR_NO_NEXT_STATE) {
            curr_state = next_state;

            if (is_string) {
                debug_scanner("curr_sym: ASCII(%d) => '%c' [STRING]\n", curr_sym, curr_sym);
                token_val_add_char(ptr_token, curr_sym);
            } else if (curr_sym != EOF && curr_sym != ' ' && curr_sym != '\n' && !end_of_comment) {
                debug_scanner("curr_sym: ASCII(%d) => '%c' \n", curr_sym, curr_sym);
                token_val_add_char(ptr_token, curr_sym);
            }
        } else {
            read_last_sym_from_previous_word = true;

            // No rule was matched
            if (is_final_state(curr_state, ptr_fa)) {
                // returns token struct (represents lexical unit)
                generate_token(ptr_token, ptr_stack, curr_state);
                return;
            } else {
                if (curr_sym != EOF) {
                    print_lex_error(&file_pos, curr_sym);
                    exit(RC_LEX_ERR); // End program with lexical error
                }
            }
        }
    }

    // End Of File -> finished successfully
    debug_scanner("End of file\n%s", "");
    generate_token(ptr_token, ptr_stack, TOKEN_EOF);
}

// Update the tracked position of current character in input file
void update_file_position(file_positionT *file_pos, char curr_sym) {
    if (curr_sym == '\n') {
        file_pos->line_number++;
        file_pos->line_char_position = 0;
    } else {
        file_pos->line_char_position++;
    }
}

// Prints lexical error message and current position in input file
void print_lex_error(file_positionT *file_pos, char curr_sym) {
    fprintf(stderr, "\nFinished at symbol: '%c' \n", curr_sym);
    fprintf(stderr, "Lexical error detected [line: %d char: %d]\n", file_pos->line_number, file_pos->line_char_position);
}

// Clears token if it is end of comment
bool is_end_of_comment(tokenT *ptr_token, int curr_state, int next_state) {
    if (curr_state == STATE_LINE_COMMENT_BODY && next_state == STATE_START) {
        debug_scanner("CLEAR TOKEN AFTER LINE COMMENT\n%s", "");
        token_clear(ptr_token);
        return true;
    }

    if (curr_state == STATE_BLOCK_COMMENT_END && next_state == STATE_START) {
        debug_scanner("CLEAR TOKEN AFTER BLOCK COMMENT\n%s", "");
        token_clear(ptr_token);
        return true;
    }

    return false;
}
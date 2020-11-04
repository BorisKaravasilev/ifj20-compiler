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

// Add identifier to sym table
void add_id_to_sym_table(tokenT *ptr_token, Stack *ptr_stack) {
    stringT *key = &ptr_token->attribute.string_val;
    Symtable *ptr_curr_scope_sym_table = stack_top(ptr_stack).symtable;
    st_insert_symbol(ptr_curr_scope_sym_table, key, 0);
    debug_scanner("\n | SYMTABLE | [INSERTED KEY: '%s' TO SYMTABLE]\n\n\n", key->string);
}

// Push new symtable block
void push_new_symtable_block(Stack *ptr_stack) {
    debug_scanner("\n | SYMTABLE | [PUSH NEW SYMTABLE BLOCK]\n\n\n%s", "");
    stack_push(ptr_stack, st_init());
}

// Pop symtable block
void pop_symtable_block(Stack *ptr_stack) {
    debug_scanner("\n | SYMTABLE | [POP SYMTABLE BLOCK]\n\n\n%s", "");
    stack_pop(ptr_stack);
}

// Update symtable according to received token
void update_symtable(tokenT *ptr_token, Stack *ptr_stack) {
    if (ptr_token->token_type == TOKEN_IDENTIFIER) {
        add_id_to_sym_table(ptr_token, ptr_stack);
    } else if (ptr_token->token_type == TOKEN_LEFT_CURLY_BRACE) {
        push_new_symtable_block(ptr_stack);
    } else if (ptr_token->token_type == TOKEN_RIGHT_CURLY_BRACE) {
        pop_symtable_block(ptr_stack);
    }
}

// Assigns correct token type and attribute to token at 'ptr_token'
void generate_token(tokenT *ptr_token, Stack *ptr_stack, int final_state) {
    ptr_token->token_type = final_state;

    if (final_state == TOKEN_IDENTIFIER) {
        // Is token keyword, build-in function or identifier?
        ptr_token->token_type = keyword_check(ptr_token);
        ptr_token->token_type = function_word_check(ptr_token);
    }

    update_symtable(ptr_token, ptr_stack);
}

// Reads new character as current symbol or uses the character from previous reading
// (Detection of token is done by reading one character past it. This character needs to
//   be used again as starting character of the next token)
char read_char(char *curr_sym, FILE *input_file, file_positionT *file_pos, bool *use_previous) {
    if (!*use_previous) {
        *curr_sym = fgetc(input_file);
        update_file_position(file_pos, *curr_sym);
    }

    *use_previous = false;
}

// Executes a transition of finite automaton, adds symbol to token and updates current state
void fa_execute_step(finite_automataT *ptr_fa, tokenT *ptr_token, fa_stepT *step) {
    // Clears token if it contains comment
    bool end_of_comment = is_end_of_comment(ptr_token, *step->curr_state, *step->next_state);

    bool is_string_sym = *step->curr_state == STATE_STRING || *step->next_state == STATE_STRING;
    // White space symbols won't be added to a token if it is not a string
    bool is_skip_sym = is_accepted(*step->curr_sym, ptr_fa->rules[SKIP_SYM_RULE_INDEX].transition_ranges);
    bool is_attribute_sym = *step->curr_sym != EOF && !is_skip_sym && !end_of_comment;

    *step->curr_state = *step->next_state; // transition

    // Check if symbol should be added to token
    if (is_string_sym) {
        debug_scanner("curr_sym: ASCII(%d) => '%c' [STRING]\n", *step->curr_sym, *step->curr_sym);
        token_val_add_char(ptr_token, *step->curr_sym);
    } else if (is_attribute_sym) {
        debug_scanner("curr_sym: ASCII(%d) => '%c' \n", *step->curr_sym, *step->curr_sym);
        token_val_add_char(ptr_token, *step->curr_sym);
    }
}

// Generates token if in final state or exits with lexical error
void handle_final_state_or_error(finite_automataT *ptr_fa, Stack *ptr_stack, tokenT *ptr_token, fa_stepT *step, file_positionT *file_pos) {
    if (is_final_state(*step->curr_state, ptr_fa)) {
        // Is final state and sets token at 'ptr_token'
        generate_token(ptr_token, ptr_stack, *step->curr_state);
    } else {
        // Exits with lexical error
        if (*step->curr_sym != EOF) {
            print_lex_error(file_pos, *step->curr_sym);
            exit(RC_LEX_ERR); // End program with lexical error
        }
    }
}

// Scans input characters and generates token, lexical error or returns 'false' if EOF.
bool scan_token(finite_automataT *ptr_fa, FILE *input_file, Stack *ptr_stack, tokenT *ptr_token) {
    int curr_state = STATE_START;
    int next_state;
    static char curr_sym = 0;
    static file_positionT file_pos = {1, 0};

    // Ensures reading of last character that belongs to next token
    static bool use_previous_sym = false;

    while (curr_sym != EOF) {
        read_char(&curr_sym, input_file, &file_pos, &use_previous_sym);

        next_state = get_next_state(curr_sym, curr_state, ptr_fa);
        fa_stepT fa_step = {&curr_state, &next_state, &curr_sym};

        // Can transition to next state
        if (next_state != ERROR_NO_NEXT_STATE) {
            // Update current state + add symbol to token
            fa_execute_step(ptr_fa, ptr_token, &fa_step);
        } else {
            // Can't transition to next state
            use_previous_sym = true;
            handle_final_state_or_error(ptr_fa,ptr_stack,ptr_token,&fa_step,&file_pos);
            return true; // Successfully generated token into 'ptr_token'
        }
    }

    return false; // Scanned EOF
}

// TODO: pass EOL flag to get_next_token()
// Generates token into 'ptr_token' or exits with lexical error
void get_next_token(finite_automataT *ptr_fa, FILE *input_file, Stack *ptr_stack, tokenT *ptr_token) {
    // Cleans the content of token_val string
    token_clear(ptr_token);

    if (scan_token(ptr_fa, input_file, ptr_stack, ptr_token)) return;

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
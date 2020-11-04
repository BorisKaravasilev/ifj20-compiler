#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "scanner_functions.h"
#include "token_types.h"
#include "states_list.h"
#include "../general/return_codes.h"
#include "../general/debugging.h"

#define ERROR_NO_NEXT_STATE -2

void init_scanner(scannerT *s, FILE *input_file) {
    s->input_fp = input_file;
    s->file_pos.line_number = 1;
    s->file_pos.line_char_position = 0;

    // Modify 'finite_automata.c' to change FA graph
    init_finite_automata(&s->fa);

    // Top level symbol table
    Symtable *global_scope_symtable = st_init();

    stack_init(&s->st_stack);
    stack_push(&s->st_stack, global_scope_symtable);

    s->required_eol_found = true;
}

void free_scanner(scannerT *s) {
    stack_free(&s->st_stack);
}

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
void generate_token(scannerT *s, tokenT *ptr_token, int final_state) {
    ptr_token->token_type = final_state;

    if (final_state == TOKEN_IDENTIFIER) {
        // Is token keyword, build-in function or identifier?
        ptr_token->token_type = keyword_check(ptr_token);
        ptr_token->token_type = function_word_check(ptr_token);
    }

    update_symtable(ptr_token, &s->st_stack);
}

// Reads new character as current symbol or uses the character from previous reading
// (Detection of token is done by reading one character past it. This character needs to
//   be used again as starting character of the next token)
char read_char(scannerT *s, char *curr_sym, bool *use_previous) {
    if (!*use_previous) {
        *curr_sym = fgetc(s->input_fp);
        update_file_position(&s->file_pos, *curr_sym);
    }

    *use_previous = false;
}


void update_eol_found_in_skip_sym(scannerT *s, fa_stepT *step, eol_flagE eol_flag) {
    if (eol_flag == REQUIRED) {
        if (*step->curr_sym == EOL) s->required_eol_found = true;
    }
    if (eol_flag == FORBIDDEN) {
        if (*step->curr_sym == EOL) {
            print_syntax_error(&s->file_pos);
            fprintf(stderr, "Unexpected end of line.\n");
            exit(RC_SYN_ERR); // Exit program with syntax error
        }
    }
}

void check_required_eol_found(scannerT *s, eol_flagE eol_flag) {
    if (eol_flag == REQUIRED) {
        if (!s->required_eol_found) {
            print_syntax_error(&s->file_pos);
            fprintf(stderr, "Required end of line not found.\n");
            exit(RC_SYN_ERR); // Exit program with syntax error
        }
    }
}

// Executes a transition of finite automaton, adds symbol to token and updates current state
void fa_execute_step(scannerT *s, tokenT *ptr_token, fa_stepT *step, eol_flagE eol_flag) {
    // Clears token if it contains comment
    bool end_of_comment = is_end_of_comment(ptr_token, *step->curr_state, *step->next_state);

    bool is_string_sym = *step->curr_state == STATE_STRING || *step->next_state == STATE_STRING;
    // White space symbols won't be added to a token if it is not a string
    bool is_skip_sym = is_accepted(*step->curr_sym, s->fa.rules[SKIP_SYM_RULE_INDEX].transition_ranges);
    bool is_attribute_sym = *step->curr_sym != EOF && !is_skip_sym && !end_of_comment;
    //bool is_end_of_skip_sequence = *step->curr_state == SKIP_SYM_RULE_INDEX;

    *step->curr_state = *step->next_state; // transition

    // Check if symbol should be added to token
    if (is_string_sym) {
        debug_scanner("curr_sym: ASCII(%d) => '%c' [STRING]\n", *step->curr_sym, *step->curr_sym);
        token_val_add_char(ptr_token, *step->curr_sym);
    } else if (is_attribute_sym) {
        debug_scanner("curr_sym: ASCII(%d) => '%c' \n", *step->curr_sym, *step->curr_sym);
        token_val_add_char(ptr_token, *step->curr_sym);
        check_required_eol_found(s, eol_flag);
    }

    if (is_skip_sym && !is_string_sym) {
        update_eol_found_in_skip_sym(s, step, eol_flag);
    }
}

// Generates token if in final state or exits with lexical error
void handle_final_state_or_error(scannerT *s, tokenT *ptr_token, fa_stepT *step) {
    if (is_final_state(*step->curr_state, &s->fa)) {
        // Is final state
        generate_token(s, ptr_token, *step->curr_state); //  set token at 'ptr_token'
    } else {
        // Not final state
        if (*step->curr_sym != EOF) {
            print_lex_error(&s->file_pos, *step->curr_sym);
            exit(RC_LEX_ERR); // Exit program with lexical error
        }
    }
}

// Scans input characters and generates token, lexical error or returns 'false' if EOF.
bool scan_token(scannerT *s, tokenT *ptr_token, eol_flagE eol_flag) {
    int curr_state = STATE_START;
    int next_state;
    static char curr_sym = 0;

    // Ensures reading of last character that belongs to next token
    static bool use_previous_sym = false;

    while (curr_sym != EOF) {
        read_char(s, &curr_sym, &use_previous_sym);

        next_state = get_next_state(curr_sym, curr_state, &s->fa);
        fa_stepT fa_step = {&curr_state, &next_state, &curr_sym};

        // Can transition to next state
        if (next_state != ERROR_NO_NEXT_STATE) {
            // Update current state + add symbol to token
            fa_execute_step(s, ptr_token, &fa_step, eol_flag);
        } else {
            // Can't transition to next state
            use_previous_sym = true;
            handle_final_state_or_error(s, ptr_token, &fa_step);
            return true; // Successfully generated token into 'ptr_token'
        }
    }

    return false; // Scanned EOF
}

// Generates token into 'ptr_token' or exits with lexical error
void get_next_token(scannerT *s, tokenT *ptr_token, eol_flagE eol_flag) {
    // Cleans the content of token_val string
    token_clear(ptr_token);
    s->required_eol_found = false; // Reset value

    if (scan_token(s, ptr_token, eol_flag)) return;

    // End Of File -> finished successfully
    debug_scanner("End of file\n%s", "");
    generate_token(s, ptr_token, TOKEN_EOF);
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
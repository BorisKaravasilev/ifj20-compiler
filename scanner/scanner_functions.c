/**
 * Project: Implementation of a compiler of the IFJ20 language.
 * @file    scanner_functions.h
 * @brief   Contains mainly functions working with structures "scannerT" and "finite_automataT"
 * @author  Boris Karavasilev <xkarav01@stud.fit.vutbr.cz>
 * @date    28. 9. 2020
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "scanner_functions.h"
#include "token_types.h"
#include "states_list.h"
#include "return_codes.h"
#include "debugging.h"

#define ERROR_NO_NEXT_STATE -2

// Initializes scanner structure
void init_scanner(scannerT *s, FILE *input_file) {
    s->input_fp = input_file;
    s->file_pos.line_number = 1;
    s->file_pos.line_char_position = 0;

    // Modify 'finite_automata.c' to change FA graph
    init_finite_automata(&s->fa);

    s->required_eol_found = true;

    s->curr_sym = 0;
    s->use_previous_sym = false;
}

// Dealocates memory from scanner members
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

// Assigns correct token type and attribute to token at 'ptr_token'
void generate_token(scannerT *s, tokenT *ptr_token, int final_state) {
    ptr_token->token_type = final_state;

    if (final_state == TOKEN_IDENTIFIER) {
        // Is token keyword, build-in function or identifier?
        ptr_token->token_type = keyword_check(ptr_token);
        ptr_token->token_type = function_word_check(ptr_token);
    } else if (final_state == TOKEN_ZERO_INTEGER_LITERAL) {
        // Zero token is also integer literal (was added extra to check leading zeroes)
        // Turn it into integer literal to not affect syntax analysis
        ptr_token->token_type = TOKEN_INTEGER_LITERAL;
    }
}

// Reads new character as current symbol or uses the character from previous reading
// (Detection of token is done by reading one character past it. This character needs to
//   be used again as starting character of the next token)
void read_char(scannerT *s) {
    if (!s->use_previous_sym) {
        s->curr_sym = fgetc(s->input_fp);
        update_file_position(&s->file_pos, s->curr_sym);
    }

    s->use_previous_sym = false;
}

// Toggles 'required_eol_found' in scanner or exits with error
void update_eol_found_in_skip_sym(scannerT *s, eol_flagE eol_flag) {
    if (eol_flag == REQUIRED) {
        if (s->curr_sym == EOL) s->required_eol_found = true;
    }
    if (eol_flag == FORBIDDEN) {
        if (s->curr_sym == EOL) {
            print_error(&s->file_pos, "Syntax");
            fprintf(stderr, "Unexpected end of line.\n");
            exit(RC_SYN_ERR); // Exit program with syntax error
        }
    }
}

// If EOL was required after token and wasn't found exits with error
void check_required_eol_found(scannerT *s, eol_flagE eol_flag) {
    if (eol_flag == REQUIRED) {
        if (!s->required_eol_found) {
            print_error(&s->file_pos, "Syntax");
            fprintf(stderr, "Required end of line not found.\n");
            exit(RC_SYN_ERR); // Exit program with syntax error
        }
    }
}

// Checks non-printable symbols after token for EOL
// and if doesn't satisfy EOL flag exits with error
void check_eol_after_token(scannerT *s, eol_flagE eol_flag) {
    int curr_state = STATE_START;
    int next_state = STATE_START;
    bool skipped_symbol;

    do {
        read_char(s);
        next_state = get_next_state(s->curr_sym, curr_state, &s->fa);
        skipped_symbol = next_state == STATE_START;

        if (skipped_symbol) {
            curr_state = next_state;
            update_eol_found_in_skip_sym(s, eol_flag);
        } else {
            s->use_previous_sym = true;
            check_required_eol_found(s, eol_flag);
        }
    } while (skipped_symbol);
}

void add_string_sym_to_token(tokenT *ptr_token, fa_stepT *step) {
    static bool is_string_start = true;
    bool is_string_end = *step->curr_state == TOKEN_STRING_LITERAL;

    if (is_string_start) {
        debug_scanner("curr_sym: ASCII(%d) => '%c' [STRING START]\n", *step->curr_sym, *step->curr_sym);
        is_string_start = false;
    } else if (is_string_end) {
        debug_scanner("curr_sym: ASCII(%d) => '%c' [STRING END]\n", *step->curr_sym, *step->curr_sym);
        is_string_start = true;
    } else {
        debug_scanner("curr_sym: ASCII(%d) => '%c' [STRING]\n", *step->curr_sym, *step->curr_sym);
        token_val_add_char(ptr_token, *step->curr_sym);
    }
}

// Executes a transition of finite automaton, adds symbol to token and updates current state
void fa_execute_step(scannerT *s, tokenT *ptr_token, fa_stepT *step) {
    // Clears token if it contains comment
    bool end_of_comment = is_end_of_comment(ptr_token, *step->curr_state, *step->next_state);

    bool is_string_sym = *step->curr_state == STATE_STRING || *step->next_state == STATE_STRING;
    // White space symbols won't be added to a token if it is not a string
    bool is_skip_sym = is_accepted(*step->curr_sym, s->fa.rules[SKIP_SYM_RULE_INDEX].transition_ranges);
    bool is_attribute_sym = *step->curr_sym != EOF && !is_skip_sym && !end_of_comment;

    *step->curr_state = *step->next_state; // transition

    // Check if symbol should be added to token
    if (is_string_sym) {
        add_string_sym_to_token(ptr_token, step);
    } else if (is_attribute_sym) {
        debug_scanner("curr_sym: ASCII(%d) => '%c' \n", *step->curr_sym, *step->curr_sym);
        token_val_add_char(ptr_token, *step->curr_sym);
    }
}

// Generates token if in final state or exits with lexical error
void handle_final_state_or_error(scannerT *s, tokenT *ptr_token, fa_stepT *step, eol_flagE eol_flag) {
    if (is_final_state(*step->curr_state, &s->fa)) {
        // Is final state
        generate_token(s, ptr_token, *step->curr_state); //  set token at 'ptr_token'
        check_eol_after_token(s, eol_flag);
    } else {
        // Not final state
        if (*step->curr_sym != EOF) {
            print_lex_error(&s->file_pos, *step->curr_sym);
            exit(RC_LEX_ERR); // Exit program with lexical error
        }
    }
}

// Checks for not closed block comment
void check_unending_block_comment(scannerT *s, int curr_state) {
    if (curr_state == STATE_BLOCK_COMMENT_BODY && s->curr_sym == EOF) {
        fprintf(stderr, "Unending block comment.\n");
        print_lex_error(&s->file_pos, s->curr_sym);
        exit(RC_LEX_ERR); // Exit program with lexical error
    }
}

// Scans input characters and generates token, lexical error or returns 'false' if EOF.
bool scan_token(scannerT *s, tokenT *ptr_token, eol_flagE eol_flag) {
    int curr_state = STATE_START;
    int next_state;

    while (s->curr_sym != EOF) {
        read_char(s);

        next_state = get_next_state(s->curr_sym, curr_state, &s->fa);
        fa_stepT fa_step = {&curr_state, &next_state, &s->curr_sym};

        // Can transition to next state
        if (next_state != ERROR_NO_NEXT_STATE) {
            // Update current state + add symbol to token
            fa_execute_step(s, ptr_token, &fa_step);
        } else {
            // Can't transition to next state
            s->use_previous_sym = true; // Ensures reading of last character that belongs to next token

            check_unending_block_comment(s, curr_state);
            handle_final_state_or_error(s, ptr_token, &fa_step, eol_flag);
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

    if (!scan_token(s, ptr_token, eol_flag)) {
        // End Of File -> finished successfully
        debug_scanner("End of file\n%s", "");
        generate_token(s, ptr_token, TOKEN_EOF);
    } else {
        debug_token(ptr_token, 0);
    }
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

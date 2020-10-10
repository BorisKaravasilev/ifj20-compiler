#ifndef __LEX_STRUCTURES_H_
#define __LEX_STRUCTURES_H_

#define STATES_LEN 50
#define ALPHABET_LEN 50
#define RULES_LEN 50
#define TRANS_SYM_LEN 50
#define FINAL_STATES_LEN 50
#define START_STATE 1

// Rule of finite automata in the format 'pa -> q'
// p = from state
// q = to state
// a = set of acceptable symbols to make a transition to 'q'
typedef struct {
    int from_state;
    char transition_symbols[TRANS_SYM_LEN]; // TODO: Change to 'range_or_charT'
    int to_state;
} ruleT;

// M = (Q, Sigma, R, s, F)
typedef struct {
    int states[STATES_LEN];
    char alphabet[ALPHABET_LEN];
    ruleT rules[RULES_LEN];
    int start_state;
    int final_states[FINAL_STATES_LEN];
} finite_automataT;

// Represents a range of characters or a single character (from, to = -1)
// - Not used variables are set to -1 (either from & to, or single_char)
typedef struct {
    char from;
    char to;
    char single_char;
} range_or_charT;

#endif
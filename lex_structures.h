#ifndef __LEX_STRUCTURES_H_
#define __LEX_STRUCTURES_H_

// TODO: Rewrite to dynamic size arrays

// Rule of finite automata in the format 'pa -> q'
// p = from state
// q = to state
// a = set of acceptable symbols to make a transition to 'q'
typedef struct {
    int from_state;
    char transition_symbols[50];
    int to_state;
} ruleT;

// M = (Q, Sigma, R, s, F)
typedef struct {
    int states[50];
    char alphabet[50];
    ruleT rules[50];
    int start_state;
    int final_states[50];
} finite_automataT;

// Represents a range of characters or a single character (from, to = -1)
// - Not used variables are set to -1 (either from & to, or single_char)
typedef struct {
    char from;
    char to;
    char single_char;
} range_or_charT;

#endif
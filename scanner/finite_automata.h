#ifndef __LEX_FINITE_AUTOMATA_H_
#define __LEX_FINITE_AUTOMATA_H_

#define STATES_LEN 50
#define ALPHABET_LEN 50
#define RULES_LEN 50
#define TRANS_SYM_LEN 50
#define FINAL_STATES_LEN 50
#define START_STATE 1
#define RANGES_IN_RULE 10

// Range of characters that the automaton accepts
typedef struct {
    char from;
    char to;
} rangeT;

// Represents:
// - one or multiple ranges of characters or multiple individual characters
// - a single character (from, to = -1)
// Not used variables are set to -1 (either from & to, or single_char)
// For multiple individual characters, their value is stored in 'from'
// and the value in 'to' is -1.
typedef struct {
    rangeT range[RANGES_IN_RULE];
    char single_char;
} range_or_charT;

// Rule of finite automata in the format 'pa -> q'
// p = from state
// q = to state
// a = set of acceptable symbols to make a transition to 'q'
typedef struct {
    int from_state;
    range_or_charT transition_symbols;
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

void init_finite_automata(finite_automataT *fa);
void init_int_array(int array[], int len, int init_value);
void init_char_array(char array[], int len, char init_value);
void init_rules_array(ruleT array[], int len, ruleT init_value);

#endif

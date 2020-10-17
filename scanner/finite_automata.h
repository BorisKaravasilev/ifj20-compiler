#ifndef __LEX_FINITE_AUTOMATA_H_
#define __LEX_FINITE_AUTOMATA_H_

#define STATES_LEN 50
#define RULES_LEN 50
#define TRANS_RANGES_LEN 50
#define FINAL_STATES_LEN 50
#define START_STATE 1


// Represents:
// - a range of characters (from, to), single_char == -1
// - a single character (single_char), from, to == -1
// Not used variables are set to -1 (either from & to, or single_char)
typedef struct {
    char from;
    char to;
    char single_char;
} range_or_charT;

// Rule of finite automata in the format 'pa -> q'
// p = from state
// q = to state
// a = set of acceptable symbols or symbol ranges to make a transition to 'q'
typedef struct {
    int from_state;
    range_or_charT transition_ranges[TRANS_RANGES_LEN];
    int to_state;
} ruleT;

typedef struct {
    int states[STATES_LEN];
    ruleT rules[RULES_LEN];
    int start_state;
    int final_states[FINAL_STATES_LEN];
} finite_automataT;

void init_finite_automata(finite_automataT *fa);
void init_int_array(int array[], int len, int init_value);
void init_rules_array(ruleT array[], int len);
void init_rule(ruleT *rule);

#endif

/**
 * Project: Implementation of a compiler of the IFJ20 language.
 * @file    finite_automata.h
 * @brief   This is the header file for finite_automata.c with definitions of constants and functions.
 * @author  Boris Karavasilev <xkarav01@stud.fit.vutbr.cz>
 * @author  Domink Vecera <xvecer23@stud.fit.vutbr.cz>
 * @author  Robin Stancl <xstanc09@stud.fit.vutbr.cz>
 * @date    4. 10. 2020
 */

#ifndef __LEX_FINITE_AUTOMATA_H_
#define __LEX_FINITE_AUTOMATA_H_

#define STATES_LEN 50 // Max.total states
#define RULES_LEN 65 // Max. rules
#define TRANS_RANGES_LEN 10 // Max. rule transition ranges
#define FINAL_STATES_LEN 35 // Max. final states
#define SKIP_SYM_RULE_INDEX 54 // Index of rule for skipping white space symbols

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
    int to_state;
    range_or_charT transition_ranges[TRANS_RANGES_LEN];
} ruleT;

typedef struct {
    int states[STATES_LEN];             // Q
    ruleT rules[RULES_LEN];             // R
    int start_state;                    // s
    int final_states[FINAL_STATES_LEN]; // F
} finite_automataT;

// Data for single step of FA
typedef struct {
    int *curr_state;
    int *next_state;
    char *curr_sym;
} fa_stepT;

void init_finite_automata(finite_automataT *fa);
void init_int_array(int array[], int len, int init_value);
void init_rules_array(ruleT array[], int len);
void init_rule(ruleT *rule);

#endif

#include "finite_automata.h"

void init_finite_automata(finite_automataT* fa) {
    // Init array with predefined values that cant occur in the array
    init_int_array(fa->final_states, FINAL_STATES_LEN, 0);
    init_int_array(fa->states, STATES_LEN, 0);
    init_rules_array(fa->rules, RULES_LEN);
    // end of initializing arrays

    // ---------------------------------------------------------------------
    // .-= FINITE AUTOMATA DEFINITION =-.
    //
    // Representation of 'finite_automata_graph.jpg'
    //
    //

    // States
    fa->states[0] = 1;  // start
    fa->states[1] = 2;  // 'a' or 'b' symbols string
    fa->states[2] = 3;  // '1' or '0' digits number

    // Start state
    fa->start_state = 1;

    // Final states
    fa->final_states[0] = 2; // 'a' or 'b' symbols string
    fa->final_states[1] = 3; // '1' or '0' digits number

    // --- Rule #0
    fa->rules[0].from_state = 1;
    fa->rules[0].to_state = 1;
    fa->rules[0].transition_ranges[0].single_char = ' ';
    fa->rules[0].transition_ranges[1].single_char = '\n';

    // --- Rule #1
    fa->rules[1].from_state = 1;
    fa->rules[1].to_state = 2;
    fa->rules[1].transition_ranges[0].from = 'a';
    fa->rules[1].transition_ranges[0].to = 'b';

    // --- Rule #2
    fa->rules[2].from_state = 2;
    fa->rules[2].to_state = 2;
    fa->rules[2].transition_ranges[0].from = 'a';
    fa->rules[2].transition_ranges[0].to = 'b';

    // --- Rule #3
    fa->rules[3].from_state = 1;
    fa->rules[3].to_state = 3;
    fa->rules[3].transition_ranges[0].from = '0';
    fa->rules[3].transition_ranges[0].to = '1';

    // --- Rule #4
    fa->rules[4].from_state = 3;
    fa->rules[4].to_state = 3;
    fa->rules[4].transition_ranges[0].from = '0';
    fa->rules[4].transition_ranges[0].to = '1';
}

void init_int_array(int int_array[], int len, int init_value) {
    for (int i = 0; i < len; i++) {
        int_array[i] = init_value;
    }
}

void init_rules_array(ruleT rules_array[], int len) {
    for (int i = 0; i < len; i++) {
        init_rule(&rules_array[i]);
    }
}

void init_rule(ruleT *rule) {
    rule->to_state = 0;
    rule->from_state = 0;

    for (int i = 0; i < TRANS_RANGES_LEN; i++) {
        rule->transition_ranges[i].from = -1;
        rule->transition_ranges[i].to = -1;
        rule->transition_ranges[i].single_char = -1;
    }
}

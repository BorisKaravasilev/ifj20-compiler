#include "finite_automata.h"


void init_finite_automata(finite_automataT* fa) {
    // Init array with predefined values that cant occur in the array
    init_int_array(fa->final_states, FINAL_STATES_LEN, 0);
    init_int_array(fa->states, STATES_LEN, 0);

    ruleT r_init;
    r_init.from_state = 0;
    r_init.to_state = 0;

    range_or_charT rc;
    rc.from = -1;
    rc.to = -1;
    rc.single_char = -1;

    for (int i = 0; i < TRANS_SYM_LEN; i++){
        r_init.transition_symbols[i] = rc;
    }

    init_rules_array(fa->rules, RULES_LEN, r_init);
    // end of initializing arrays

    // ---------------------------------------------------------------------
    // .-= FINITE AUTOMATA DEFINITION =-.
    //
    // Representation of 'finite_automata_graph.jpg'
    //
    // TODO: Set rules through FA indexes and initialize at once

    // --- Rule #1
    ruleT r1;
    init_rule(&r1);

    r1.from_state = 1;
    r1.to_state = 1;
    r1.transition_symbols[0].single_char = ' ';
    r1.transition_symbols[1].single_char = '\n';

    // --- Rule #2
    ruleT r2;
    init_rule(&r2);

    r2.from_state = 1;
    r2.to_state = 2;
    r2.transition_symbols[0].from = 'a';
    r2.transition_symbols[0].to = 'b';

    // --- Rule #3
    ruleT r3;
    init_rule(&r3);

    r3.from_state = 2;
    r3.to_state = 2;
    r3.transition_symbols[0].from = 'a';
    r3.transition_symbols[0].to = 'b';

    // --- Rule #4
    ruleT r4;
    init_rule(&r4);

    r4.from_state = 1;
    r4.to_state = 3;
    r4.transition_symbols[0].from = '0';
    r4.transition_symbols[0].to = '1';

    // --- Rule #5
    ruleT r5;
    init_rule(&r5);

    r5.from_state = 3;
    r5.to_state = 3;
    r5.transition_symbols[0].from = '0';
    r5.transition_symbols[0].to = '1';

    // Finite automata definition
    fa->states[0] = 1;  // start
    fa->states[1] = 2;  // 'a' or 'b' symbols string
    fa->states[2] = 3;  // '1' or '0' digits number

    fa->start_state = 1;
    fa->final_states[0] = 2;
    fa->final_states[1] = 3;
    fa->rules[0] = r1;
    fa->rules[1] = r2;
    fa->rules[2] = r3;
    fa->rules[3] = r4;
    fa->rules[4] = r5;
}

void init_int_array(int array[], int len, int init_value) {
    for (int i = 0; i < len; i++) {
        array[i] = init_value;
    }
}

void init_rules_array(ruleT array[], int len, ruleT init_value) {
    for (int i = 0; i < len; i++) {
        array[i] = init_value;
    }
}

void init_rule(ruleT *rule) {
    rule->to_state = 0;
    rule->from_state = 0;

    for (int i = 0; i < TRANS_SYM_LEN; i++) {
        rule->transition_symbols[i].from = -1;
        rule->transition_symbols[i].to = -1;
        rule->transition_symbols[i].single_char = -1;
    }
}

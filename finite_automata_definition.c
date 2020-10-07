#include "lex_structures.h"

void init_int_array(int array[], int len, int init_value);
void init_char_array(char array[], int len, char init_value);
void init_rules_array(ruleT array[], int len, ruleT init_value);

void init_finite_automata(finite_automataT* fa) {
    // TODO: Change 50 to "STATES_LENGTH" constant
    // Init array with predefined values that cant occur in the array
    // normally
    init_int_array(fa->final_states, 50, 0);
    init_int_array(fa->states, 50, 0);
    init_char_array(fa->alphabet, 50, '~');

    ruleT r_init;
    r_init.from_state = 0;
    r_init.to_state = 0;
    init_char_array(r_init.transition_symbols, 50, '~');
    init_rules_array(fa->rules, 50, r_init);
    // end of initializing arrays

    // --- Rule #1
    ruleT r1;

    r1.from_state = 1;
    r1.to_state = 1;
    r1.transition_symbols[0] = ' ';
    r1.transition_symbols[1] = '\n';

    // --- Rule #2
    ruleT r2;

    r2.from_state = 1;
    r2.to_state = 2;
    r2.transition_symbols[0] = 'a';
    r2.transition_symbols[1] = 'b';

    // --- Rule #3
    ruleT r3;

    r3.from_state = 2;
    r3.to_state = 2;
    r3.transition_symbols[0] = 'a';
    r3.transition_symbols[1] = 'b';

    // --- Rule #4
    ruleT r4;

    r4.from_state = 1;
    r4.to_state = 3;
    r4.transition_symbols[0] = '0';
    r4.transition_symbols[1] = '1';

    // --- Rule #5
    ruleT r5;

    r5.from_state = 3;
    r5.to_state = 3;
    r5.transition_symbols[0] = '0';
    r5.transition_symbols[1] = '1';

    // Finite automata definition
    fa->states[0] = 1;  // start
    fa->states[1] = 2;  // 'a' or 'b' symbols string
    fa->states[2] = 3;  // '1' or '0' digits number

    fa->alphabet[0] = 'a';
    fa->alphabet[1] = 'b';
    fa->alphabet[2] = '0';
    fa->alphabet[3] = '1';

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

void init_char_array(char array[], int len, char init_value) {
    for (int i = 0; i < len; i++) {
        array[i] = init_value;
    }
}

void init_rules_array(ruleT array[], int len, ruleT init_value) {
    for (int i = 0; i < len; i++) {
        array[i] = init_value;
    }
}
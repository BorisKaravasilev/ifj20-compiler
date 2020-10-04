#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "lex_functions.h"
#include "lex_structures.h"

void choose_FA(char start_sym, finite_automataT *fa);

// All FA
finite_automataT fa[2];
int fa_len = sizeof(fa) / sizeof(finite_automataT);
int rules_len = sizeof(fa[0].rules) / sizeof(ruleT);
int trans_sym_len = sizeof(fa[0].rules[0].transition_symbols) / sizeof(char);

int curr_state = 1;
finite_automataT *curr_fa = NULL;

int main() {
    printf("Hello from B-LEX \n");
    char curr_sym;

    // Rule #1
    ruleT r1;

    r1.from_state = 1;
    r1.to_state = 2;
    r1.transition_symbols[0] = '0';
    r1.transition_symbols[1] = '1';

    // Rule #2
    ruleT r2;

    r2.from_state = 1;
    r2.to_state = 2;
    r2.transition_symbols[0] = 'a';
    r2.transition_symbols[1] = 'b';

    // Finite automata #1
    strcpy(fa[0].name, "integer");
    fa[0].states[0] = 1;
    fa[0].states[1] = 2;

    fa[0].alphabet[0] = '0';
    fa[0].alphabet[1] = '1';

    fa[0].start_state = 1;
    fa[0].final_states[0] = 2;
    fa[0].rules[0] = r1;

    // Finite automata #2
    strcpy(fa[1].name, "identifier");
    fa[1].states[0] = 1;
    fa[1].states[1] = 2;

    fa[1].alphabet[0] = 'a';
    fa[1].alphabet[1] = 'b';

    fa[1].start_state = 1;
    fa[1].final_states[0] = 2;
    fa[1].rules[0] = r2;

    int step_count = 0;

    do {
        curr_sym = get_next_sym();
        // printf("Step %d | symbol: '%c' %d \n", step_count, curr_sym,
        // curr_sym);

        if (curr_sym != EOF) {
            choose_FA(curr_sym, fa);
        }

        step_count++;

    } while (curr_sym != EOF);

    return 0;
}

void choose_FA(char curr_sym, finite_automataT *fa) {
    // Go through all FA
    for (int i = 0; i < fa_len; i++) {
        // printf("FA name: %s\n", fa[i].name);

        // Go through all rules in FA
        for (int j = 0; j < rules_len; j++) {
            // Is rule initialized (valid / in use)?
            if (fa[i].rules[j].from_state >= 1) {
                // printf("%d. rule FROM state: %d\n", j,
                // fa[i].rules[j].from_state);
                // Rule for current state?
                if (fa[i].rules[j].from_state == curr_state) {
                    // Go through all transition symbols in rule
                    for (int k = 0; k < trans_sym_len; k++) {
                        if (fa[i].rules[j].transition_symbols[k] == curr_sym) {
                            int next_state = fa[i].rules[j].to_state;
                            printf("Going from state %d to state %d\n",
                                   curr_state, next_state);
                            // Go to next state according to rule
                            curr_state = next_state;

                            // Set FA for current lexical unit (have to be
                            // deterministic)
                            curr_fa = &fa[i];
                            printf("Chosen FA: %s\n", fa[i].name);
                        }
                    }
                }
            }
        }
        printf("Rules: %d\n", rules_len);
    }
}
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "lex_functions.h"
#include "lex_structures.h"

void choose_FA(char start_sym, finite_automataT *fa);

// All FA

finite_automataT fa[2];
// int fa_len = sizeof(fa) / sizeof(finite_automataT);
// int rules_len = sizeof(fa[0].rules) / sizeof(ruleT);
// int trans_sym_len = sizeof(fa[0].rules[0].transition_symbols) / sizeof(char);

int curr_state = 1;
finite_automataT *curr_fa = NULL;

int main() {
    printf("Hello from B-LEX \n\n");
    char curr_sym;

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

    // Finite automata #1
    strcpy(fa[0].name, "integer");

    // Finite automata #2
    strcpy(fa[1].name, "old_name_to_be_removed");
    fa[1].states[0] = 1;  // start
    fa[1].states[1] = 2;  // 'a' or 'b' symbols string
    fa[1].states[2] = 3;  // 1 or 0 digits number

    fa[1].alphabet[0] = 'a';
    fa[1].alphabet[1] = 'b';
    fa[1].alphabet[2] = '0';
    fa[1].alphabet[3] = '1';

    fa[1].start_state = 1;
    fa[1].final_states[0] = 2;
    fa[1].final_states[1] = 3;
    fa[1].rules[0] = r1;
    fa[1].rules[1] = r2;
    fa[1].rules[2] = r3;
    fa[1].rules[3] = r4;
    fa[1].rules[4] = r5;

    int step_count = 0;

    // int next_state = get_next_state('a', 1, &fa[1]);
    // printf("NEXT STATE IN MAIN: %d \n", next_state);

    int token = 0;
    int counter = 0;

    while (token != -1 && token != -2) {
        counter++;
        token = get_next_token(&fa[1]);

        if (token == 2) {
            printf("TOKEN #%d | type %d | 'a' or 'b' letters\n", counter,
                   token);
        } else if (token == 3) {
            printf("TOKEN #%d | type %d | '0' or '1' digits\n", counter, token);
        }
    }

    printf("Lex finished. \n");

    // do {
    //     // Parser will be calling get_next_token()

    //     curr_sym = get_next_sym();
    //     // printf("Step %d | symbol: '%c' %d \n", step_count, curr_sym,
    //     // curr_sym);

    //     if (curr_sym != EOF) {
    //         choose_FA(curr_sym, fa);
    //     }

    //     step_count++;

    // } while (curr_sym != EOF);

    return 0;
}

// void choose_FA(char curr_sym, finite_automataT *fa) {
//     // Go through all FA
//     for (int i = 0; i < fa_len; i++) {
//         // printf("FA name: %s\n", fa[i].name);

//         // Go through all rules in FA
//         for (int j = 0; j < rules_len; j++) {
//             // Is rule initialized (valid / in use)?
//             if (fa[i].rules[j].from_state >= 1) {
//                 // printf("%d. rule FROM state: %d\n", j,
//                 // fa[i].rules[j].from_state);
//                 // Rule for current state?
//                 if (fa[i].rules[j].from_state == curr_state) {
//                     // Go through all transition symbols in rule
//                     for (int k = 0; k < trans_sym_len; k++) {
//                         if (fa[i].rules[j].transition_symbols[k] == curr_sym)
//                         {
//                             int next_state = fa[i].rules[j].to_state;
//                             printf("Going from state %d to state %d\n",
//                                    curr_state, next_state);
//                             // Go to next state according to rule
//                             curr_state = next_state;

//                             // Set FA for current lexical unit (have to be
//                             // deterministic)
//                             curr_fa = &fa[i];
//                             printf("Chosen FA: %s\n", fa[i].name);
//                         }
//                     }
//                 }
//             }
//         }
//         printf("Rules: %d\n", rules_len);
//     }
// }

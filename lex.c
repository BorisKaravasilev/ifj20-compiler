// Boris Karavasilev <xkarav01@stud.fit.vutbr.cz>

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "finite_automata_definition.h"
#include "lex_functions.h"
#include "lex_structures.h"

int main() {
    printf("Universal finite automata processor B-LEX \n\n");

    finite_automataT fa;
    // For FA definition check 'finite_automata_definition.c' and
    // 'finite_automata_graph.jpg'
    init_finite_automata(&fa);

    int token = 0;
    int counter = 0;

    while (token != -1 && token != -2) {
        counter++;
        token = get_next_token(&fa);

        // Deciding which final state the processing ended at
        if (token == 2) {
            printf("TOKEN #%d | type %d | 'a' or 'b' letters\n", counter,
                   token);
        } else if (token == 3) {
            printf("TOKEN #%d | type %d | '0' or '1' digits\n", counter, token);
        }
    }

    return 0;
}

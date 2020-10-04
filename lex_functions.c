#include "lex_functions.h"

#include <stdbool.h>
#include <stdio.h>

// Global variables

// Symbols that should be ignored (eg., blank spaces, line endings ...)
char skip_symbols[] = {' ', '\n'};
int skip_symbols_len = sizeof(skip_symbols) / sizeof(char);

// Returns next valid symbol from 'stdin'
char get_next_sym() {
    char next_sym;
    bool is_skip_sym;

    do {
        is_skip_sym = false;
        next_sym = getchar();

        // Check if current symbol is a skip symbol
        for (int i = 0; i < skip_symbols_len; i++) {
            if (skip_symbols[i] == next_sym) {
                is_skip_sym = true;
                break;
            }
        }

        if (!is_skip_sym) {
            return next_sym;
        }
    } while (next_sym != EOF);

    return EOF;
}

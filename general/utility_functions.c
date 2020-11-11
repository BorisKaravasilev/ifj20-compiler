//
// Created by Boris on 09-Oct-20.
//

#include "utility_functions.h"

// Returns pointer to file or 'stdin'
FILE *get_input_file(bool enabled, char *file_name) {
    FILE *fp;

    if (enabled) {
        fp = fopen(file_name, "r");

        if (fp == NULL) {
            fprintf(stderr, "Input file doesn't exist\n");
        } else {
            return fp;
        }
    }

    return stdin;
}

// Update the tracked position of current character in input file
void update_file_position(file_positionT *file_pos, char curr_sym) {
    if (curr_sym == '\n') {
        file_pos->line_number++;
        file_pos->line_char_position = 0;
    } else {
        file_pos->line_char_position++;
    }
}

// Prints lexical error message and current position in input file
void print_lex_error(file_positionT *file_pos, char curr_sym) {
    if (curr_sym == '\r') {
        fprintf(stderr, "\nFinished at line break '\\r' \n");
    } else if (curr_sym == '\n' ) {
        fprintf(stderr, "\nFinished at line break '\\n' \n");
    } else {
        fprintf(stderr, "\nFinished at symbol: '%c' \n", curr_sym);
    }
    fprintf(stderr, "Lexical error detected [line: %d char: %d]\n", file_pos->line_number, file_pos->line_char_position);
}

// Prints lexical error message and current position in input file
void print_syntax_error(file_positionT *file_pos) {
    fprintf(stderr, "\nSyntax error detected [line: %d char: %d]\n", file_pos->line_number, file_pos->line_char_position);
}

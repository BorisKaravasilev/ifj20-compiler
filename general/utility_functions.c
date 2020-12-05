//
// Created by Boris on 09-Oct-20.
//

#include <stdlib.h>
#include "return_codes.h"
#include "utility_functions.h"

// Returns pointer to file or 'stdin'
FILE *get_input_file(bool enabled, char *file_name) {
    FILE *fp;

    if (enabled) {
        fp = fopen(file_name, "r");

        if (fp == NULL) {
            fprintf(stderr, "Input file doesn't exist\n");
            exit(RC_RUN_ERR);
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

// Prints general error message and current position in input file
void print_error(file_positionT *file_pos, char *error_type_name) {
    fprintf(stderr, "\n%s error detected [line: %d char: %d]\n", error_type_name, file_pos->line_number, file_pos->line_char_position);
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

void print_error_message_by_code(int return_code, file_positionT *file_position) {
    if (return_code == RC_SYN_ERR) {
        print_error(file_position, "Syntax");
    } else if (return_code >= 3 && return_code <= 7) {
        print_error(file_position, "Semantic");
    } else if (return_code == RC_ZERO_DIVISION_ERR) {
        print_error(file_position, "Zero division");
    } else if (return_code == RC_RUN_ERR) {
        print_error(file_position, "Runtime");
    }
}

bool check_hex(int c){
    if ((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || c >= 'A' && c <= 'F')
        return true;
    else
        return false;
}
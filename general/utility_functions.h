//
// Created by Boris on 09-Oct-20.
//

#ifndef IFJ20_COMPILER_UTILITY_FUNCTIONS_H
#define IFJ20_COMPILER_UTILITY_FUNCTIONS_H

#include <stdio.h>
#include <stdbool.h>

// Position of last read char from input file
typedef struct {
    int line_number;
    int line_char_position; // position of char on current line (starting at 1)
} file_positionT;

FILE *get_input_file(bool enabled, char *file_name);
void update_file_position(file_positionT *file_pos, char curr_sym);
void print_error(file_positionT *file_pos, char *error_type_name);
void print_lex_error(file_positionT *file_pos, char curr_sym);
void print_error_message_by_code(int return_code, file_positionT *file_position);
bool check_hex(int c);

#endif //IFJ20_COMPILER_UTILITY_FUNCTIONS_H

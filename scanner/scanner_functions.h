#ifndef __LEX_FUNCTIONS_H_
#define __LEX_FUNCTIONS_H_

#include <stdio.h>
#include "finite_automata.h"
#include "token_functions.h"
#include "stack.h"
#include "symtable.h"

typedef struct {
    int line_number;
    int line_char_position;
} file_positionT;

void get_next_token(finite_automataT *ptr_fa, FILE *input_file, Stack *ptr_stack, tokenT *ptr_token);
void token_array_free(tokenT *ptr_token_array, int array_length);
bool is_accepted(char sym, range_or_charT transition_ranges[]);
void update_file_position(file_positionT *file_pos, char curr_sym);
void print_lex_error(file_positionT *file_pos, char curr_sym);
bool is_end_of_comment(tokenT *ptr_token, int curr_state, int next_state);

#endif

#ifndef __LEX_FUNCTIONS_H_
#define __LEX_FUNCTIONS_H_

#include <stdio.h>
#include "finite_automata.h"
#include "token_functions.h"

void get_next_token(finite_automataT *ptr_fa, FILE *input_file, symtableT *ptr_symtable, tokenT *ptr_token);
void token_array_free(tokenT *ptr_token_array, int array_length);
bool is_accepted(char sym, range_or_charT transition_ranges[]);

#endif

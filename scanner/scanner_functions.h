#ifndef __LEX_FUNCTIONS_H_
#define __LEX_FUNCTIONS_H_

#include <stdio.h>
#include "finite_automata.h"
#include "token_functions.h"

void get_next_token(finite_automataT *ptr_fa, FILE *input_file, symtableT *ptr_symtable, tokenT *ptr_token);

#endif
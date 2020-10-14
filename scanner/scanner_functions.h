#ifndef __LEX_FUNCTIONS_H_
#define __LEX_FUNCTIONS_H_

#include <stdio.h>
#include "finite_automata.h"
#include "token_functions.h"

void get_next_token(finite_automataT *ptr_fa, FILE *ptr_input_file, tokenT *ptr_token);

#endif
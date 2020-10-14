#ifndef __LEX_FUNCTIONS_H_
#define __LEX_FUNCTIONS_H_

#include <stdio.h>
#include "finite_automata.h"
#include "token_functions.h"

tokenT get_next_token(finite_automataT *fa, FILE *input_file, tokenT *token);

#endif
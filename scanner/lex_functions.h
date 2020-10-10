#ifndef __LEX_FUNCTIONS_H_
#define __LEX_FUNCTIONS_H_

#include <stdio.h>
#include "lex_structures.h"

int get_next_token(finite_automataT *fa, FILE *input_file);

#endif
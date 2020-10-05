#ifndef __LEX_FUNCTIONS_H_
#define __LEX_FUNCTIONS_H_

#include "lex_structures.h"

char get_next_sym();
int get_next_state(char curr_sym, int curr_state, finite_automataT *fa);
int get_next_token(finite_automataT *fa);

#endif
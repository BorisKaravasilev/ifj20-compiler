#ifndef __LEX_FUNCTIONS_H_
#define __LEX_FUNCTIONS_H_

#include <stdio.h>
#include <stdbool.h>
#include "lex_structures.h"
#include "token_functions.h"

token_struct get_next_token(finite_automataT *fa, FILE *input_file, token_struct *token);
bool is_accepted(char sym, range_or_charT rule_symbols);

#endif
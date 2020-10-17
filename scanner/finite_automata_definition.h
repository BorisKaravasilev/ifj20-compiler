#ifndef __LEX_FINITE_AUTOMATA_DEFINITION_H_
#define __LEX_FINITE_AUTOMATA_DEFINITION_H_

#include "lex_structures.h"

void init_finite_automata(finite_automataT *fa);

void init_int_array(int array[], int len, int init_value);
void init_char_array(char array[], int len, char init_value);
void init_rules_array(ruleT array[], int len, ruleT init_value);

#endif
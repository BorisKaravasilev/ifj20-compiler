#ifndef __LEX_FUNCTIONS_H_
#define __LEX_FUNCTIONS_H_

#include <stdio.h>
#include "finite_automata.h"
#include "token_functions.h"
#include "stack.h"
#include "symtable.h"
#include "../general/utility_functions.h"

#define EOL '\n'

// EOL can, can't or has to be present before next token
typedef enum {
    OPTIONAL,
    FORBIDDEN,
    REQUIRED
} eol_flagE;

typedef struct {
    FILE *input_fp;         // Input file pointer
    file_positionT file_pos;// Position of last read character by scanner
    finite_automataT fa;    // Finite automaton
    Stack st_stack;         // Stack of symbol tables representing different scopes
    bool required_eol_found;// Indicates if EOL required by EOL flag has been found in non-printable chars before token
} scannerT;

void init_scanner(scannerT *s, FILE *input_file);
void free_scanner(scannerT *s);
void get_next_token(scannerT *s, tokenT *ptr_token, eol_flagE eol_flag);
void token_array_free(tokenT *ptr_token_array, int array_length);
bool is_accepted(char sym, range_or_charT transition_ranges[]);
int get_next_state(char curr_sym, int curr_state, finite_automataT *ptr_fa);
int try_rule_transition(char sym, int state, ruleT *rule);
bool is_end_of_comment(tokenT *ptr_token, int curr_state, int next_state);

#endif

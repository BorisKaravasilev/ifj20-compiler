/**
 * Project: Implementation of a compiler of the IFJ20 language.
 * @brief   Infix to postfix expression conversion header file
 * @author  Dominik Vecera <xvecer23@stud.fit.vutbr.cz>
 * @author  Petr Vrtal <xvrtal01@stud.fit.vutbr.cz>
 * @author  Robin Stancl <xstanc09@stud.fit.vutbr.cz>

 */

#ifndef IFJ_PROJEKT_INFIX2POSTFIX_H
#define IFJ_PROJEKT_INFIX2POSTFIX_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "parser.h"
#include "scanner.h"

#define STACK_SIZE 50

typedef struct {
    tToken arr[STACK_SIZE];                /* array for values */
    int top;                                /* index of element on top of stack */
} tStack;

void s_init ( tStack* s );
int s_empty ( const tStack* s );
int s_full ( const tStack* s );
tToken s_top ( const tStack* s );
tToken s_pop ( tStack* s );
void s_push ( tStack* s, tToken t );

bool is_operator ( tToken t );
int priority ( tToken t );
int create_expr_list ( tToken* start, tListToken* out_list, tToken last_token );
void expr_to_postfix ( tListToken* in_expr, tListToken* out_list );
void infix_to_postfix ( tListToken* inf_expr, tListToken* post_expr );

#endif //IFJ_PROJEKT_INFIX2POSTFIX_H

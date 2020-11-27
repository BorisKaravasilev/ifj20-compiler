/**
 * Project: Implementation of a compiler of the IFJ20 language.
 * @brief   Infix to postfix expression conversion header file
 * @author  Dominik Vecera <xvecer23@stud.fit.vutbr.cz>
 * @author  Petr Vrtal <xvrtal01@stud.fit.vutbr.cz>
 * @author  Robin Stancl <xstanc09@stud.fit.vutbr.cz>

 *//*


#ifndef IFJ_PROJEKT_INFIX2POSTFIX_H
#define IFJ_PROJEKT_INFIX2POSTFIX_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "parser.h"
#include "scanner_functions.h"
#include "token_functions.h"
#include "token_linked_list.h"

#define STACK_SIZE 100

typedef struct {
    tokenT arr[STACK_SIZE];                */
/* array for values *//*

    int top;                                */
/* index of element on top of stack *//*

} tStack;

void s_init ( tStack* s );
int s_empty ( const tStack* s );
int s_full ( const tStack* s );
tokenT *s_top (const tStack* s);
tokenT s_pop ( tStack* s );
void s_push ( tStack* s, tokenT t );

bool is_operator ( tokenT *t );
int priority ( tokenT *t );
int create_expr_list ( tokenT* start, token_listT* out_list, tokenT last_token );
void expr_to_postfix ( token_listT* in_expr, token_listT* out_list );
void infix_to_postfix ( token_listT* inf_expr, token_listT* post_expr );

#endif //IFJ_PROJEKT_INFIX2POSTFIX_H
*/

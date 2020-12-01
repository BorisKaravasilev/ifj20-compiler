/**
 * Project: Implementation of a compiler of the IFJ20 language.
 * @brief   Infix to postfix expression conversion header file
 * @author  Dominik Vecera <xvecer23@stud.fit.vutbr.cz>
 * @author  Petr Vrtal <xvrtal01@stud.fit.vutbr.cz>
 * @author  Robin Stancl <xstanc09@stud.fit.vutbr.cz>

 */


#ifndef IFJ20_COMPILER_PARSER_H_INFIX2POSTFIX_H
#define IFJ20_COMPILER_PARSER_H_INFIX2POSTFIX_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "parser.h"
#include "scanner_functions.h"
#include "token_functions.h"
#include "token_linked_list.h"

typedef struct {
    struct token_list_itemT *top;
    int size;
} tStack;

void s_init ( tStack* s );
bool s_empty ( const tStack* s );
token_list_itemT *s_pop ( tStack* s );
void s_push ( tStack* s, token_list_itemT *new );

bool is_operator ( token_list_itemT *t );
int priority ( token_list_itemT *t );
int create_expr_list ( tokenT* start, token_listT* out_list, tokenT last_token );
void infix_to_postfix ( token_listT* in_expr, token_listT* out_list );

#endif // IFJ20_COMPILER_PARSER_H_INFIX2POSTFIX_H

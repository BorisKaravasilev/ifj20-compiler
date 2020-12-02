/**
 * Project: Implementation of a compiler of the IFJ20 language.
 * @brief   Infix to postfix expression conversion
 * @author  Dominik Vecera <xvecer23@stud.fit.vutbr.cz>
 * @author  Petr Vrtal <xvrtal01@stud.fit.vutbr.cz>
 * @author  Robin Stancl <xstanc09@stud.fit.vutbr.cz>
 */


#include "infix2postfix.h"

void s_init (tStack* s) {
    if (s != NULL) {
        s->top = NULL;
        s->size = 0;
    }
}

bool s_empty (const tStack* s) {
    return (s->top == NULL);
}

token_list_itemT *s_pop (tStack* s) {
    if (!s_empty(s)) {
        token_list_itemT *temp = s->top;
        s->top = s->top->next;
        s->size--;
        return temp;
    }
    return NULL;
}

void s_push (tStack* s, token_list_itemT *new) {
    if (s != NULL || new != NULL) {
        token_list_itemT *item = token_list_item_init(&new->token);
        item->next = s->top;
        s->top = item;
        s->size++;
    }
}

bool is_operator (token_list_itemT *t) {
    switch(t->token.token_type) {
        case TOKEN_PLUS:
        case TOKEN_MINUS:
        case TOKEN_MULTIPLICATION:
        case TOKEN_DIVISION:
        case TOKEN_LEFT_ARROW:
        case TOKEN_RIGHT_ARROW:
        case TOKEN_NOT_EQUAL:
        case TOKEN_LESS_OR_EQUAL:
        case TOKEN_GREATER_OR_EQUAL:
        case TOKEN_DOUBLE_EQUAL:
            return true;

        default:
            return false;
    }
}

int priority (token_list_itemT *t) {
    switch(t->token.token_type) {
        case TOKEN_MULTIPLICATION:
        case TOKEN_DIVISION:
            return 1;

        case TOKEN_PLUS:
        case TOKEN_MINUS:
            return 2;

        case TOKEN_LEFT_ARROW:
        case TOKEN_RIGHT_ARROW:
        case TOKEN_NOT_EQUAL:
        case TOKEN_LESS_OR_EQUAL:
        case TOKEN_GREATER_OR_EQUAL:
        case TOKEN_DOUBLE_EQUAL:
            return 3;

        default:
            return 0;
    }
}

void infix_to_postfix (token_listT* in_expr, token_listT* out_list) {
    tStack stack;
    s_init(&stack);

    tokenT right_par_token;
    token_init(&right_par_token);
    right_par_token.token_type = TOKEN_RIGHT_BRACKET;
    token_list_itemT *right_bracket = token_list_item_init(&right_par_token);

    tokenT left_par_token;
    token_init(&left_par_token);
    left_par_token.token_type = TOKEN_LEFT_BRACKET;
    token_list_itemT *left_bracket = token_list_item_init(&left_par_token);

    token_list_add_item(in_expr, &right_par_token);
    s_push(&stack, left_bracket);

    token_list_itemT *p;
    token_list_itemT *item = in_expr->first;

    while (item != NULL) {
        if (item->token.token_type == TOKEN_LEFT_BRACKET) {
            s_push(&stack, item);
        } else if (item->token.token_type == TOKEN_IDENTIFIER || item->token.token_type == TOKEN_STRING_LITERAL ||
                   item->token.token_type == TOKEN_INTEGER_LITERAL || item->token.token_type == TOKEN_DECIMAL_LITERAL || item->token.token_type == TOKEN_EXPONENT_LITERAL) {
            token_list_add_item(out_list, &item->token);
        } else if (is_operator(item) == true) {
            p = s_pop(&stack);
            while (is_operator(p) == true && priority(p) >= priority(item)) {
                token_list_add_item(out_list, &p->token);
                p = s_pop(&stack);
            }
            s_push(&stack, p);
            s_push(&stack, item);
        } else if (item->token.token_type == TOKEN_RIGHT_BRACKET) {
            p = s_pop(&stack);
            while (p->token.token_type != TOKEN_LEFT_BRACKET) {
                token_list_add_item(out_list, &p->token);
                p = s_pop(&stack);
            }
        }

        item = item->next;
    }
}

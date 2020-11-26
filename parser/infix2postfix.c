/**
 * Project: Implementation of a compiler of the IFJ20 language.
 * @brief   Infix to postfix expression conversion
 * @author  Dominik Vecera <xvecer23@stud.fit.vutbr.cz>
 * @author  Petr Vrtal <xvrtal01@stud.fit.vutbr.cz>
 * @author  Robin Stancl <xstanc09@stud.fit.vutbr.cz>
 */

#include "infix2postfix.h"
#include "orient_express.h"

void s_init (tStack* s) {
    if (s != NULL) {
        s->top = -1;

        int i;
        for (i = 0; i < STACK_SIZE; i++) {
            s->arr[i] = NULL;
        }
    }
}

int s_empty (const tStack* s) {
    return (s->top == -1) ? 1 : 0;
}

int s_full (const tStack* s) {
    return (s->top >= STACK_SIZE - 1) ? 1 : 0;
}

tToken s_top (const tStack* s) {
    if (!s_empty(s)) {
        return s->arr[s->top];
    }
    return NULL;
}

tToken s_pop (tStack* s) {
    if (!s_empty(s)) {
        tToken temp = s_top(s);
        s->top--;
        return temp;
    }
    return NULL;
}

void s_push (tStack* s, tToken t) {
    if (!s_full(s)) {
        s->top++;
        s->arr[s->top] = t;
    }
}

bool is_operator (tToken t) {
    switch(t->token) {
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

int priority (tToken t) {
    switch(t->token) {
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

int create_expr_list (tToken* start, tListToken* out_list, tToken last_token) {
    tToken item = *start;
    while (item->token != TOKEN_EXPRESSION_END) {
        if (item->token != TOKEN_EXPRESSION_BEGIN) {
            string_copy(&str, &item->token_string);
            add_token_to_list(out_list, item->token);
        }
        item = item->next;

        if (item == NULL) {
            return result;
        }
    }
    last_token->token = item->token;
    last_token->token_string = item->token_string;
    last_token->next = item->next;

    add_token_to_list(out_list, TOKEN_RIGHT_BRACKET);

    string_copy(&str, &item->token_string);
    add_token_to_list(out_list, item->token); // add back TOKEN_EXPR_END to the list for another loop
    return 0;
}

void expr_to_postfix (tListToken* in_expr, tListToken* out_list) {
    tStack stack; // vytvoreni a inicializace stacku
    s_init(&stack);

    tToken left_par_token; // vytvorneni tokenu pro "("
    left_par_token = token_init(left_par_token, TOKEN_LEFT_BRACKET);

    s_push(&stack, left_par_token); // pushnuti leve zavorky do stacku

    tToken p;
    tToken item = in_expr->First;

    while (item->token != TOKEN_EXPRESSION_END) {
        if (item->token == TOKEN_LEFT_BRACKET) {
            s_push(&stack, item);
        } else if (item->token == TOKEN_IDENTIFIER || item->token == TOKEN_STRING_LITERAL ||
                   item->token == TOKEN_KEY_WORD_NONE || item->token == TOKEN_INTEGER_LITERAL ||
                   item->token == TOKEN_DECIMAL_LITERAL || item->token == TOKEN_EXPONENT_LITERAL) {
            string_copy(&str, &item->token_string);
            add_token_to_list(out_list, item->token);
        } else if (is_operator(item) == true) {
            p = s_pop(&stack);
            while (is_operator(p) == true && priority(p) >= priority(item)) {
                string_copy(&str, &p->token_string);
                add_token_to_list(out_list, p->token);
                p = s_pop(&stack);
            }
            s_push(&stack, p);
            s_push(&stack, item);
        } else if (item->token == TOKEN_RIGHT_BRACKET) {
            p = s_pop(&stack);
            while (p->token != TOKEN_LEFT_BRACKET) {
                string_copy(&str, &p->token_string);
                add_token_to_list(out_list, p->token);
                p = s_pop(&stack);
            }
        } else {
            //DEBUG ONLY!!
            printf("\nInvalid token.\n");
            getchar();
            exit(1);
        }

        item = item->next;
    }
    string_copy(&str, &item->token_string);
    add_token_to_list(out_list, TOKEN_EXPRESSION_END);
}

void infix_to_postfix (tListToken* inf_expr, tListToken* post_expr) {
    tToken item = inf_expr->First;
    tListToken expr_list;
    list_token_initialize(&expr_list);

    while (item != NULL) {
        if (item->token == TOKEN_EXPRESSION_BEGIN) {
            list_token_initialize(&expr_list);
            string_copy(&str, &item->token_string);
            add_token_to_list(post_expr, item->token); // vlozime zpět TOKEN_EXPR_BEGIN

            tToken last = token_init(last, TOKEN_PLUS);
            int res = create_expr_list(&item, &expr_list, last);
            if (res != 0) {
                break;
            }
            expr_to_postfix(&expr_list, post_expr);
            item = last->next; // skip pres vyraz az na TOKEN_EXPR_END
            continue;
        }
        string_copy(&str, &item->token_string); // copy content of token
        add_token_to_list(post_expr, item->token);
        item = item->next;
    }

    dispose_list_of_tokens(&expr_list);
}
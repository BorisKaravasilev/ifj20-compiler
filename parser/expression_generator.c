/**
 * Project: Implementation of a compiler of the IFJ20 language.
 * @file    expression_generator.c
 * @brief   This file contains the implementation of the function for generating the IFJcode20 for expressions.
 * @author  Robin Stancl <xstanc09@stud.fit.vutbr.cz>
 */

#include "expression_generator.h"

///Function for generating the IFJcode20 for the expression
int generate_expression(token_listT *token_list_generate, int division_type)
{
    ///First we need the new temporary frame
    gen_createframe();
    ///Variable result_here in the TF for saving the result
    gen_defvar(EXPR_RESULT_HERE);

    ///Variable for moving in the structure of tokens
    struct token_list_itemT *token_pointer = token_list_generate->first;

    ///Initialization of the stack
    int number_of_variables_on_stack = 0;
    int name_of_first_free_variable = 1;
    int size_of_stack = 1;
    int *expr_stack = malloc(sizeof(int) * size_of_stack);
    ///If malloc return NULL -> RC_RUN_ERR
    if (expr_stack == NULL)
    {
        return RC_RUN_ERR;
    }

    while (1)
    {
        ///What is current token?
        if (token_pointer->token.token_type == TOKEN_INTEGER_LITERAL)
        {
            ///Adding variable to the stack
            if (number_of_variables_on_stack == size_of_stack)
            {
                ///Realloc
                size_of_stack++;
                int *expr_stack_2 = realloc(expr_stack, sizeof(int) * size_of_stack);
                if (expr_stack_2 == NULL)
                {
                    return RC_RUN_ERR;
                }
                expr_stack = expr_stack_2;
            }
            expr_stack[number_of_variables_on_stack] = name_of_first_free_variable;

            ///DEFVAR
            printf("DEFVAR TF@E%d\n", name_of_first_free_variable);
            ///MOVE
            printf("MOVE TF@E%d int@%s\n", name_of_first_free_variable, token_pointer->token.attribute.string_val.string);

            number_of_variables_on_stack++;
            name_of_first_free_variable++;
        }
        else if ((token_pointer->token.token_type == TOKEN_DECIMAL_LITERAL) || (token_pointer->token.token_type == TOKEN_EXPONENT_LITERAL))
        {
            ///Adding variable to the stack
            if (number_of_variables_on_stack == size_of_stack)
            {
                ///Realloc
                size_of_stack++;
                int *expr_stack_2 = realloc(expr_stack, sizeof(int) * size_of_stack);
                if (expr_stack_2 == NULL)
                {
                    return RC_RUN_ERR;
                }
                expr_stack = expr_stack_2;
            }
            expr_stack[number_of_variables_on_stack] = name_of_first_free_variable;

            ///DEFVAR
            printf("DEFVAR TF@E%d\n", name_of_first_free_variable);

            ///MOVE
            printf("MOVE TF@E%d float@", name_of_first_free_variable);

            ///Float must have special format
            char float_new_format[1000];
            sprintf(float_new_format, "%a", strtof(token_pointer->token.attribute.string_val.string, NULL));
            printf("%s\n", float_new_format);

            number_of_variables_on_stack++;
            name_of_first_free_variable++;
        }
        else if (token_pointer->token.token_type == TOKEN_STRING_LITERAL)
        {
            ///Adding variable to the stack
            if (number_of_variables_on_stack == size_of_stack)
            {
                ///Realloc
                size_of_stack++;
                int *expr_stack_2 = realloc(expr_stack, sizeof(int) * size_of_stack);
                if (expr_stack_2 == NULL)
                {
                    return RC_RUN_ERR;
                }
                expr_stack = expr_stack_2;
            }
            expr_stack[number_of_variables_on_stack] = name_of_first_free_variable;

            ///DEFVAR
            printf("DEFVAR TF@E%d\n", name_of_first_free_variable);
            ///MOVE
            printf("MOVE TF@E%d string@%s\n", name_of_first_free_variable, token_pointer->token.attribute.string_val.string);

            number_of_variables_on_stack++;
            name_of_first_free_variable++;
        }
        else if (token_pointer->token.token_type == TOKEN_IDENTIFIER)
        {
            ///Adding variable to the stack
            if (number_of_variables_on_stack == size_of_stack)
            {
                ///Realloc
                size_of_stack++;
                int *expr_stack_2 = realloc(expr_stack, sizeof(int) * size_of_stack);
                if (expr_stack_2 == NULL)
                {
                    return RC_RUN_ERR;
                }
                expr_stack = expr_stack_2;
            }
            expr_stack[number_of_variables_on_stack] = name_of_first_free_variable;

            ///DEFVAR
            printf("DEFVAR TF@E%d\n", name_of_first_free_variable);
            ///MOVE
            printf("MOVE TF@E%d LF@%s\n", name_of_first_free_variable, token_pointer->token.attribute.string_val.string);

            number_of_variables_on_stack++;
            name_of_first_free_variable++;
        }
        else if (token_pointer->token.token_type == TOKEN_PLUS)
        {
            ///Adding variable to the stack
            if (number_of_variables_on_stack == size_of_stack)
            {
                ///Realloc
                size_of_stack++;
                int *expr_stack_2 = realloc(expr_stack, sizeof(int) * size_of_stack);
                if (expr_stack_2 == NULL)
                {
                    return RC_RUN_ERR;
                }
                expr_stack = expr_stack_2;
            }
            expr_stack[number_of_variables_on_stack] = name_of_first_free_variable;

            ///DEFVAR
            printf("DEFVAR TF@E%d\n", name_of_first_free_variable);
            number_of_variables_on_stack++;
            ///ADD
            printf("ADD TF@E%d TF@E%d TF@E%d\n", name_of_first_free_variable, expr_stack[(number_of_variables_on_stack - 3)], expr_stack[(number_of_variables_on_stack - 2)]);

            expr_stack[(number_of_variables_on_stack - 3)] = expr_stack[(number_of_variables_on_stack - 1)];

            number_of_variables_on_stack = number_of_variables_on_stack - 2;
            name_of_first_free_variable++;
        }
        else if (token_pointer->token.token_type == TOKEN_MINUS)
        {
            ///Adding variable to the stack
            if (number_of_variables_on_stack == size_of_stack)
            {
                ///Realloc
                size_of_stack++;
                int *expr_stack_2 = realloc(expr_stack, sizeof(int) * size_of_stack);
                if (expr_stack_2 == NULL)
                {
                    return RC_RUN_ERR;
                }
                expr_stack = expr_stack_2;
            }
            expr_stack[number_of_variables_on_stack] = name_of_first_free_variable;

            ///DEFVAR
            printf("DEFVAR TF@E%d\n", name_of_first_free_variable);
            number_of_variables_on_stack++;
            ///SUB
            printf("SUB TF@E%d TF@E%d TF@E%d\n", name_of_first_free_variable, expr_stack[(number_of_variables_on_stack - 3)], expr_stack[(number_of_variables_on_stack - 2)]);

            expr_stack[(number_of_variables_on_stack - 3)] = expr_stack[(number_of_variables_on_stack - 1)];

            number_of_variables_on_stack = number_of_variables_on_stack - 2;
            name_of_first_free_variable++;
        }
        else if (token_pointer->token.token_type == TOKEN_MULTIPLICATION)
        {
            ///Adding variable to the stack
            if (number_of_variables_on_stack == size_of_stack)
            {
                ///Realloc
                size_of_stack++;
                int *expr_stack_2 = realloc(expr_stack, sizeof(int) * size_of_stack);
                if (expr_stack_2 == NULL)
                {
                    return RC_RUN_ERR;
                }
                expr_stack = expr_stack_2;
            }
            expr_stack[number_of_variables_on_stack] = name_of_first_free_variable;

            ///DEFVAR
            printf("DEFVAR TF@E%d\n", name_of_first_free_variable);
            number_of_variables_on_stack++;
            ///MUL
            printf("MUL TF@E%d TF@E%d TF@E%d\n", name_of_first_free_variable, expr_stack[(number_of_variables_on_stack - 3)], expr_stack[(number_of_variables_on_stack - 2)]);

            expr_stack[(number_of_variables_on_stack - 3)] = expr_stack[(number_of_variables_on_stack - 1)];

            number_of_variables_on_stack = number_of_variables_on_stack - 2;
            name_of_first_free_variable++;
        }
        else if (token_pointer->token.token_type == TOKEN_DIVISION)
        {
            ///Adding variable to the stack
            if (number_of_variables_on_stack == size_of_stack)
            {
                ///Realloc
                size_of_stack++;
                int *expr_stack_2 = realloc(expr_stack, sizeof(int) * size_of_stack);
                if (expr_stack_2 == NULL)
                {
                    return RC_RUN_ERR;
                }
                expr_stack = expr_stack_2;
            }
            expr_stack[number_of_variables_on_stack] = name_of_first_free_variable;

            ///DEFVAR
            printf("DEFVAR TF@E%d\n", name_of_first_free_variable);
            number_of_variables_on_stack++;

            ///If division type is 1, it is float division -> DIV, else it is int division -> IDIV
            if (division_type == 1)
            {
                ///DIV
                printf("DIV TF@E%d TF@E%d TF@E%d\n", name_of_first_free_variable, expr_stack[(number_of_variables_on_stack - 3)], expr_stack[(number_of_variables_on_stack - 2)]);
            }
            else
            {
                ///IDIV
                printf("IDIV TF@E%d TF@E%d TF@E%d\n", name_of_first_free_variable, expr_stack[(number_of_variables_on_stack - 3)], expr_stack[(number_of_variables_on_stack - 2)]);
            }

            expr_stack[(number_of_variables_on_stack - 3)] = expr_stack[(number_of_variables_on_stack - 1)];

            number_of_variables_on_stack = number_of_variables_on_stack - 2;
            name_of_first_free_variable++;
        }
        else if (token_pointer->token.token_type == TOKEN_LEFT_ARROW)
        {
            ///Adding variable to the stack
            if (number_of_variables_on_stack == size_of_stack)
            {
                ///Realloc
                size_of_stack++;
                int *expr_stack_2 = realloc(expr_stack, sizeof(int) * size_of_stack);
                if (expr_stack_2 == NULL)
                {
                    return RC_RUN_ERR;
                }
                expr_stack = expr_stack_2;
            }
            expr_stack[number_of_variables_on_stack] = name_of_first_free_variable;

            ///DEFVAR
            printf("DEFVAR TF@E%d\n", name_of_first_free_variable);
            number_of_variables_on_stack++;
            ///LT
            printf("LT TF@E%d TF@E%d TF@E%d\n", name_of_first_free_variable, expr_stack[(number_of_variables_on_stack - 3)], expr_stack[(number_of_variables_on_stack - 2)]);

            expr_stack[(number_of_variables_on_stack - 3)] = expr_stack[(number_of_variables_on_stack - 1)];

            number_of_variables_on_stack = number_of_variables_on_stack - 2;
            name_of_first_free_variable++;
        }
        else if (token_pointer->token.token_type == TOKEN_RIGHT_ARROW)
        {
            ///Adding variable to the stack
            if (number_of_variables_on_stack == size_of_stack)
            {
                ///Realloc
                size_of_stack++;
                int *expr_stack_2 = realloc(expr_stack, sizeof(int) * size_of_stack);
                if (expr_stack_2 == NULL)
                {
                    return RC_RUN_ERR;
                }
                expr_stack = expr_stack_2;
            }
            expr_stack[number_of_variables_on_stack] = name_of_first_free_variable;

            ///DEFVAR
            printf("DEFVAR TF@E%d\n", name_of_first_free_variable);
            number_of_variables_on_stack++;
            ///GT
            printf("GT TF@E%d TF@E%d TF@E%d\n", name_of_first_free_variable, expr_stack[(number_of_variables_on_stack - 3)], expr_stack[(number_of_variables_on_stack - 2)]);

            expr_stack[(number_of_variables_on_stack - 3)] = expr_stack[(number_of_variables_on_stack - 1)];

            number_of_variables_on_stack = number_of_variables_on_stack - 2;
            name_of_first_free_variable++;
        }
        else if (token_pointer->token.token_type == TOKEN_NOT_EQUAL)
        {
            ///First is ==
            ///Adding variable to the stack
            if (number_of_variables_on_stack == size_of_stack)
            {
                ///Realloc
                size_of_stack++;
                int *expr_stack_2 = realloc(expr_stack, sizeof(int) * size_of_stack);
                if (expr_stack_2 == NULL)
                {
                    return RC_RUN_ERR;
                }
                expr_stack = expr_stack_2;
            }
            expr_stack[number_of_variables_on_stack] = name_of_first_free_variable;

            ///DEFVAR
            printf("DEFVAR TF@E%d\n", name_of_first_free_variable);
            number_of_variables_on_stack++;
            ///EQ
            printf("EQ TF@E%d TF@E%d TF@E%d\n", name_of_first_free_variable, expr_stack[(number_of_variables_on_stack - 3)], expr_stack[(number_of_variables_on_stack - 2)]);

            expr_stack[(number_of_variables_on_stack - 3)] = expr_stack[(number_of_variables_on_stack - 1)];

            number_of_variables_on_stack = number_of_variables_on_stack - 2;
            name_of_first_free_variable++;

            ///Now NOT
            ///Adding variable to the stack
            if (number_of_variables_on_stack == size_of_stack)
            {
                ///Realloc
                size_of_stack++;
                int *expr_stack_2 = realloc(expr_stack, sizeof(int) * size_of_stack);
                if (expr_stack_2 == NULL)
                {
                    return RC_RUN_ERR;
                }
                expr_stack = expr_stack_2;
            }
            expr_stack[number_of_variables_on_stack] = name_of_first_free_variable;

            ///DEFVAR
            printf("DEFVAR TF@E%d\n", name_of_first_free_variable);
            number_of_variables_on_stack++;
            ///NOT
            printf("NOT TF@E%d TF@E%d\n", name_of_first_free_variable, expr_stack[(number_of_variables_on_stack - 2)]);

            expr_stack[(number_of_variables_on_stack - 2)] = expr_stack[(number_of_variables_on_stack - 1)];

            number_of_variables_on_stack = number_of_variables_on_stack - 1;
            name_of_first_free_variable++;
        }
        else if (token_pointer->token.token_type == TOKEN_LESS_OR_EQUAL)
        {
            ///First LT
            ///Adding variable to the stack
            if (number_of_variables_on_stack == size_of_stack)
            {
                ///Realloc
                size_of_stack++;
                int *expr_stack_2 = realloc(expr_stack, sizeof(int) * size_of_stack);
                if (expr_stack_2 == NULL)
                {
                    return RC_RUN_ERR;
                }
                expr_stack = expr_stack_2;
            }
            expr_stack[number_of_variables_on_stack] = name_of_first_free_variable;

            ///DEFVAR
            printf("DEFVAR TF@E%d\n", name_of_first_free_variable);
            number_of_variables_on_stack++;
            ///LT
            printf("LT TF@E%d TF@E%d TF@E%d\n", name_of_first_free_variable, expr_stack[(number_of_variables_on_stack - 3)], expr_stack[(number_of_variables_on_stack - 2)]);

            name_of_first_free_variable++;

            ///Now EQ
            ///Adding variable to the stack
            if (number_of_variables_on_stack == size_of_stack)
            {
                ///Realloc
                size_of_stack++;
                int *expr_stack_2 = realloc(expr_stack, sizeof(int) * size_of_stack);
                if (expr_stack_2 == NULL)
                {
                    return RC_RUN_ERR;
                }
                expr_stack = expr_stack_2;
            }
            expr_stack[number_of_variables_on_stack] = name_of_first_free_variable;

            ///DEFVAR
            printf("DEFVAR TF@E%d\n", name_of_first_free_variable);
            number_of_variables_on_stack++;
            ///EQ
            printf("EQ TF@E%d TF@E%d TF@E%d\n", name_of_first_free_variable, expr_stack[(number_of_variables_on_stack - 4)], expr_stack[(number_of_variables_on_stack - 3)]);

            name_of_first_free_variable++;

            ///Now OR
            ///Adding variable to the stack
            if (number_of_variables_on_stack == size_of_stack)
            {
                ///Realloc
                size_of_stack++;
                int *expr_stack_2 = realloc(expr_stack, sizeof(int) * size_of_stack);
                if (expr_stack_2 == NULL)
                {
                    return RC_RUN_ERR;
                }
                expr_stack = expr_stack_2;
            }
            expr_stack[number_of_variables_on_stack] = name_of_first_free_variable;

            ///DEFVAR
            printf("DEFVAR TF@E%d\n", name_of_first_free_variable);
            number_of_variables_on_stack++;
            ///OR
            printf("OR TF@E%d TF@E%d TF@E%d\n", name_of_first_free_variable, expr_stack[(number_of_variables_on_stack - 3)], expr_stack[(number_of_variables_on_stack - 2)]);

            expr_stack[(number_of_variables_on_stack - 5)] = expr_stack[(number_of_variables_on_stack - 1)];

            number_of_variables_on_stack = number_of_variables_on_stack - 4;
            name_of_first_free_variable++;
        }
        else if (token_pointer->token.token_type == TOKEN_GREATER_OR_EQUAL)
        {
            ///First GT
            ///Adding variable to the stack
            if (number_of_variables_on_stack == size_of_stack)
            {
                ///Realloc
                size_of_stack++;
                int *expr_stack_2 = realloc(expr_stack, sizeof(int) * size_of_stack);
                if (expr_stack_2 == NULL)
                {
                    return RC_RUN_ERR;
                }
                expr_stack = expr_stack_2;
            }
            expr_stack[number_of_variables_on_stack] = name_of_first_free_variable;

            ///DEFVAR
            printf("DEFVAR TF@E%d\n", name_of_first_free_variable);
            number_of_variables_on_stack++;
            ///GT
            printf("GT TF@E%d TF@E%d TF@E%d\n", name_of_first_free_variable, expr_stack[(number_of_variables_on_stack - 3)], expr_stack[(number_of_variables_on_stack - 2)]);

            name_of_first_free_variable++;

            ///Now EQ
            ///Adding variable to the stack
            if (number_of_variables_on_stack == size_of_stack)
            {
                ///Realloc
                size_of_stack++;
                int *expr_stack_2 = realloc(expr_stack, sizeof(int) * size_of_stack);
                if (expr_stack_2 == NULL)
                {
                    return RC_RUN_ERR;
                }
                expr_stack = expr_stack_2;
            }
            expr_stack[number_of_variables_on_stack] = name_of_first_free_variable;

            ///DEFVAR
            printf("DEFVAR TF@E%d\n", name_of_first_free_variable);
            number_of_variables_on_stack++;
            ///EQ
            printf("EQ TF@E%d TF@E%d TF@E%d\n", name_of_first_free_variable, expr_stack[(number_of_variables_on_stack - 4)], expr_stack[(number_of_variables_on_stack - 3)]);

            name_of_first_free_variable++;

            ///Now OR
            ///Adding variable to the stack
            if (number_of_variables_on_stack == size_of_stack)
            {
                ///Realloc
                size_of_stack++;
                int *expr_stack_2 = realloc(expr_stack, sizeof(int) * size_of_stack);
                if (expr_stack_2 == NULL)
                {
                    return RC_RUN_ERR;
                }
                expr_stack = expr_stack_2;
            }
            expr_stack[number_of_variables_on_stack] = name_of_first_free_variable;

            ///DEFVAR
            printf("DEFVAR TF@E%d\n", name_of_first_free_variable);
            number_of_variables_on_stack++;
            ///OR
            printf("OR TF@E%d TF@E%d TF@E%d\n", name_of_first_free_variable, expr_stack[(number_of_variables_on_stack - 3)], expr_stack[(number_of_variables_on_stack - 2)]);

            expr_stack[(number_of_variables_on_stack - 5)] = expr_stack[(number_of_variables_on_stack - 1)];

            number_of_variables_on_stack = number_of_variables_on_stack - 4;
            name_of_first_free_variable++;
        }
        else if (token_pointer->token.token_type == TOKEN_DOUBLE_EQUAL)
        {
            ///Adding variable to the stack
            if (number_of_variables_on_stack == size_of_stack)
            {
                ///Realloc
                size_of_stack++;
                int *expr_stack_2 = realloc(expr_stack, sizeof(int) * size_of_stack);
                if (expr_stack_2 == NULL)
                {
                    return RC_RUN_ERR;
                }
                expr_stack = expr_stack_2;
            }
            expr_stack[number_of_variables_on_stack] = name_of_first_free_variable;

            ///DEFVAR
            printf("DEFVAR TF@E%d\n", name_of_first_free_variable);
            number_of_variables_on_stack++;
            ///EQ
            printf("EQ TF@E%d TF@E%d TF@E%d\n", name_of_first_free_variable, expr_stack[(number_of_variables_on_stack - 3)], expr_stack[(number_of_variables_on_stack - 2)]);

            expr_stack[(number_of_variables_on_stack - 3)] = expr_stack[(number_of_variables_on_stack - 1)];

            number_of_variables_on_stack = number_of_variables_on_stack - 2;
            name_of_first_free_variable++;
        }

        ///Next token, please
        if (token_pointer->next == NULL)
        {
            ///Saving result to EXPR_RESULT_HERE
            printf("MOVE %s TF@E%d\n", EXPR_RESULT_HERE, expr_stack[(number_of_variables_on_stack - 1)]);
            free(expr_stack);
            break;
        }
        else
        {
            token_pointer = token_pointer->next;
        }
    }
}

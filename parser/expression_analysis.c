/**
 * Project: Implementation of a compiler of the IFJ20 language.
 * @file    expression_analysis.c
 * @brief   This file contains the implementation of the parsing function for expressions.
 * @author  Robin Stancl <xstanc09@stud.fit.vutbr.cz>
 * @date    28. 10. 2020
 */

#include "expression_analysis.h"
#include "token_types.h"
#include "token_linked_list.h"
#include "infix2postfix.h"

///The function for check if the token is an operator -,*,<,>,<=,>=
int operator_check(tokenT *ptr_tok_num)
{
    ///If the token is operator -,*,<,>,<=,>= function returns 1 else it returns 0
    if ((ptr_tok_num->token_type == TOKEN_MINUS) || (ptr_tok_num->token_type == TOKEN_MULTIPLICATION) || (ptr_tok_num->token_type == TOKEN_LEFT_ARROW) ||
        (ptr_tok_num->token_type == TOKEN_RIGHT_ARROW) || (ptr_tok_num->token_type == TOKEN_LESS_OR_EQUAL) || (ptr_tok_num->token_type == TOKEN_GREATER_OR_EQUAL))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

///The parsing function for the expressions (detailed description can be found in the "expression_analysis.h")
int expr_check(tokenT *ptr_identifier_token, tokenT *ptr_start_token, tokenT *ptr_last_token, tokenT *ptr_expr_data_and_type, scannerT *scanner)
{
    token_listT token_list;
    token_list_initialize(&token_list);

    switch_case = 0;
    number_of_brackets = 0;

    ptr_expr_data_and_type->token_type = EXPRESSION_NO_TYPE;
    string_clear(&ptr_expr_data_and_type->attribute.string_val);
    string_add_character(&ptr_expr_data_and_type->attribute.string_val, "");

    is_it_first_token_of_expression = 0;
    semantic_symtable_symbol = NULL;

    ///If ptr_identifier_token is not NULL, start token is after identifier - we must change switch_case to identifier (5)
    if (ptr_identifier_token != NULL)
    {
        switch_case = 3;
        ///Now we must decide if the token is identifier
        if (ptr_identifier_token->token_type == TOKEN_IDENTIFIER)
        {
            ///Now we change ptr_expr_data_and_type.token_type to identifier data type
            semantic_symtable_symbol = stack_search(&scanner->st_stack, &ptr_identifier_token->attribute.string_val);
            if (semantic_symtable_symbol == NULL)
            {
                token_list_free(&token_list);
                return RC_SEMANTIC_IDENTIFIER_ERR;
            }
            ///If semantic_data_type contains int
            if (semantic_symtable_symbol->type == TYPE_INT)
            {
                ptr_expr_data_and_type->token_type = EXPRESSION_INT;
            }
                ///If semantic_data_type contains float
            else if (semantic_symtable_symbol->type == TYPE_DECIMAL)
            {
                ptr_expr_data_and_type->token_type = EXPRESSION_FLOAT64;
            }
                ///If semantic_data_type contains string
            else if (semantic_symtable_symbol->type == TYPE_STRING)
            {
                ptr_expr_data_and_type->token_type = EXPRESSION_STRING;
            }
            else
            {
                token_list_free(&token_list);
                return RC_SEMANTIC_TYPE_COMPATIBILITY_ERR;
            }
        }
        else if (ptr_identifier_token->token_type == TOKEN_STRING_LITERAL)
        {
            ///If the first token is string literal, next switch case must be case 2 (after string)
            switch_case = 2;
        }
        ///Now we have to add this identifier token to the list of tokens
        token_list_add_item(&token_list, ptr_identifier_token);
    }

    ///Now let`s make a loop with always true condition where we will go through the expression itself
    while (1)
    {
        switch (switch_case)
        {
            ///Case 0 = First token or operator +,==,!= or left bracket
            case 0:
            {
                ///Is it the first token of the expression?
                if (is_it_first_token_of_expression != 0)
                {
                    ///Load the token
                    get_next_token(scanner, ptr_last_token, OPTIONAL);
                }
                else
                {
                    ptr_last_token = ptr_start_token;
                    is_it_first_token_of_expression++;
                }
                ///We save the token to the structure of tokens
                token_list_add_item(&token_list, ptr_last_token);

                ///What is first token?
                if (ptr_last_token->token_type == RC_LEX_ERR)
                {
                    token_list_free(&token_list);
                    return RC_LEX_ERR;
                }
                else if (ptr_last_token->token_type == RC_RUN_ERR)
                {
                    token_list_free(&token_list);
                    return RC_RUN_ERR;
                }
                else if (ptr_last_token->token_type == TOKEN_LEFT_BRACKET)
                {
                    switch_case = 0;
                    number_of_brackets++;
                }
                else if (ptr_last_token->token_type == TOKEN_STRING_LITERAL)
                {
                    switch_case = 2;
                    ///If the variable expr_data_type is not nil or string it is syntax error
                    if ((ptr_expr_data_and_type->token_type == EXPRESSION_NO_TYPE) || (ptr_expr_data_and_type->token_type == EXPRESSION_STRING))
                    {
                        ptr_expr_data_and_type->token_type = EXPRESSION_STRING;
                    }
                    else
                    {
                        token_list_free(&token_list);
                        return RC_SEMANTIC_TYPE_COMPATIBILITY_ERR;
                    }
                }
                else if ((ptr_last_token->token_type == TOKEN_INTEGER_LITERAL) || (ptr_last_token->token_type == TOKEN_DECIMAL_LITERAL) || (ptr_last_token->token_type == TOKEN_EXPONENT_LITERAL))
                {
                    switch_case = 3;
                    ///If the number is integer literal its data type is integer
                    ///If the number is decimal literal or number with exponent its data type is float
                    if (ptr_last_token->token_type == TOKEN_INTEGER_LITERAL)
                    {
                        ///If expr_data_type variable contains nil or int
                        if ((ptr_expr_data_and_type->token_type == EXPRESSION_NO_TYPE) || (ptr_expr_data_and_type->token_type == EXPRESSION_INT))
                        {
                            ptr_expr_data_and_type->token_type = EXPRESSION_INT;
                        }
                        else
                        {
                            token_list_free(&token_list);
                            return RC_SEMANTIC_TYPE_COMPATIBILITY_ERR;
                        }
                    }
                    else if ((ptr_last_token->token_type == TOKEN_DECIMAL_LITERAL) || (ptr_last_token->token_type == TOKEN_EXPONENT_LITERAL))
                    {
                        ///If expr_data_type contains nil or float
                        if ((ptr_expr_data_and_type->token_type == EXPRESSION_NO_TYPE) || (ptr_expr_data_and_type->token_type == EXPRESSION_FLOAT64))
                        {
                            ptr_expr_data_and_type->token_type = EXPRESSION_FLOAT64;
                        }
                        else
                        {
                            token_list_free(&token_list);
                            return RC_SEMANTIC_TYPE_COMPATIBILITY_ERR;
                        }
                    }
                    else
                    {
                        token_list_free(&token_list);
                        return RC_SEMANTIC_TYPE_COMPATIBILITY_ERR;
                    }
                }
                else if (ptr_last_token->token_type == TOKEN_IDENTIFIER)
                {
                    switch_case = 3;

                    semantic_symtable_symbol = stack_search(&scanner->st_stack, &ptr_last_token->attribute.string_val);
                    if (semantic_symtable_symbol == NULL)
                    {
                        token_list_free(&token_list);
                        return RC_SEMANTIC_IDENTIFIER_ERR;
                    }
                    ///If semantic_data_type contains int
                    if (semantic_symtable_symbol->type == TYPE_INT)
                    {
                        ///If ptr_expr_data_and_type->token_type is nil or int
                        if ((ptr_expr_data_and_type->token_type == EXPRESSION_NO_TYPE) || (ptr_expr_data_and_type->token_type == EXPRESSION_INT))
                        {
                            ptr_expr_data_and_type->token_type = EXPRESSION_INT;
                        }
                        else
                        {
                            token_list_free(&token_list);
                            return RC_SEMANTIC_TYPE_COMPATIBILITY_ERR;
                        }
                    }
                    ///If semantic_data_type contains float
                    else if (semantic_symtable_symbol->type == TYPE_DECIMAL)
                    {
                        ///If expr_data_type is nil or int or float
                        if ((ptr_expr_data_and_type->token_type == EXPRESSION_NO_TYPE) || (ptr_expr_data_and_type->token_type == EXPRESSION_FLOAT64))
                        {
                            ptr_expr_data_and_type->token_type = EXPRESSION_FLOAT64;
                        }
                        else
                        {
                            token_list_free(&token_list);
                            return RC_SEMANTIC_TYPE_COMPATIBILITY_ERR;
                        }
                    }
                    ///If semantic_data_type contains string
                    else if (semantic_symtable_symbol->type == TYPE_STRING)
                    {
                        ///If expr_data_type is nil or string
                        if ((ptr_expr_data_and_type->token_type == EXPRESSION_NO_TYPE) || (ptr_expr_data_and_type->token_type == EXPRESSION_STRING))
                        {
                            ptr_expr_data_and_type->token_type = EXPRESSION_STRING;
                        }
                        else
                        {
                            token_list_free(&token_list);
                            return RC_SEMANTIC_TYPE_COMPATIBILITY_ERR;
                        }
                    }
                    else
                    {
                        token_list_free(&token_list);
                        return RC_SEMANTIC_TYPE_COMPATIBILITY_ERR;
                    }
                }
                else
                {
                    token_list_free(&token_list);
                    return RC_SYN_ERR;
                }
            }
            break;

            ///Case 1 = Operators (without +, /)
            case 1:
            {
                ///Load the token
                get_next_token(scanner, ptr_last_token, OPTIONAL);
                ///We save the token to the structure of tokens
                if (number_of_token > TOKEN_ARRAY_LEN)
                {
                    token_list_free(&token_list);
                    return RC_RUN_ERR;
                }
                token_list_add_item(&token_list, ptr_last_token);

                ///What is next token?
                if (ptr_last_token->token_type == RC_LEX_ERR)
                {
                    token_list_free(&token_list);
                    return RC_LEX_ERR;
                }
                else if (ptr_last_token->token_type == RC_RUN_ERR)
                {
                    token_list_free(&token_list);
                    return RC_RUN_ERR;
                }
                else if (ptr_last_token->token_type == TOKEN_LEFT_BRACKET)
                {
                    switch_case = 0;
                    number_of_brackets++;
                }
                else if ((ptr_last_token->token_type == TOKEN_INTEGER_LITERAL) || (ptr_last_token->token_type == TOKEN_DECIMAL_LITERAL) || (ptr_last_token->token_type == TOKEN_EXPONENT_LITERAL))
                {
                    switch_case = 3;
                    ///If the number is integer literal its data type is integer
                    ///If the number is decimal literal or number with exponent its data type is float
                    if (ptr_last_token->token_type == TOKEN_INTEGER_LITERAL)
                    {
                        ///If expr_data_type variable contains nil or int
                        if ((ptr_expr_data_and_type->token_type == EXPRESSION_NO_TYPE) || (ptr_expr_data_and_type->token_type == EXPRESSION_INT))
                        {
                            ptr_expr_data_and_type->token_type = EXPRESSION_INT;
                        }
                        else
                        {
                            token_list_free(&token_list);
                            return RC_SEMANTIC_TYPE_COMPATIBILITY_ERR;
                        }
                    }
                    else if ((ptr_last_token->token_type == TOKEN_DECIMAL_LITERAL) || (ptr_last_token->token_type == TOKEN_EXPONENT_LITERAL))
                    {
                        ///If expr_data_type contains nil, int or float
                        if ((ptr_expr_data_and_type->token_type == EXPRESSION_NO_TYPE) || (ptr_expr_data_and_type->token_type == EXPRESSION_FLOAT64))
                        {
                            ptr_expr_data_and_type->token_type = EXPRESSION_FLOAT64;
                        }
                        else
                        {
                            token_list_free(&token_list);
                            return RC_SEMANTIC_TYPE_COMPATIBILITY_ERR;
                        }
                    }
                    else
                    {
                        token_list_free(&token_list);
                        return RC_SEMANTIC_TYPE_COMPATIBILITY_ERR;
                    }
                }
                else if (ptr_last_token->token_type == TOKEN_IDENTIFIER)
                {
                    switch_case = 3;
                    semantic_symtable_symbol = stack_search(&scanner->st_stack, &ptr_last_token->attribute.string_val);
                    if (semantic_symtable_symbol == NULL)
                    {
                        token_list_free(&token_list);
                        return RC_SEMANTIC_IDENTIFIER_ERR;
                    }
                    ///If semantic_data_type contains int
                    if (semantic_symtable_symbol->type == TYPE_INT)
                    {
                        ///If expr_data_type is nil or int
                        if ((ptr_expr_data_and_type->token_type == EXPRESSION_NO_TYPE) || (ptr_expr_data_and_type->token_type == EXPRESSION_INT))
                        {
                            ptr_expr_data_and_type->token_type = EXPRESSION_INT;
                        }
                        else
                        {
                            token_list_free(&token_list);
                            return RC_SEMANTIC_TYPE_COMPATIBILITY_ERR;
                        }
                    }
                    ///If semantic_data_type contains float
                    else if (semantic_symtable_symbol->type == TYPE_DECIMAL)
                    {
                        ///If expr_data_type is nil or int or float
                        if ((ptr_expr_data_and_type->token_type == EXPRESSION_NO_TYPE) || (ptr_expr_data_and_type->token_type == EXPRESSION_FLOAT64))
                        {
                            ptr_expr_data_and_type->token_type = EXPRESSION_FLOAT64;
                        }
                        else
                        {
                            token_list_free(&token_list);
                            return RC_SEMANTIC_TYPE_COMPATIBILITY_ERR;
                        }
                    }
                    else
                    {
                        token_list_free(&token_list);
                        return RC_SEMANTIC_TYPE_COMPATIBILITY_ERR;
                    }
                }
                else
                {
                    token_list_free(&token_list);
                    return RC_SYN_ERR;
                }
            }
            break;

            ///Case 2 = String
            case 2:
            {
                ///Load the token
                get_next_token(scanner, ptr_last_token, OPTIONAL);
                ///We save the token to the structure of tokens
                if (number_of_token > TOKEN_ARRAY_LEN)
                {
                    token_list_free(&token_list);
                    return RC_RUN_ERR;
                }

                ///What is the next token?
                if (ptr_last_token->token_type == RC_LEX_ERR)
                {
                    token_list_free(&token_list);
                    return RC_LEX_ERR;
                }
                else if (ptr_last_token->token_type == RC_RUN_ERR)
                {
                    token_list_free(&token_list);
                    return RC_RUN_ERR;
                }
                else if (ptr_last_token->token_type == TOKEN_EOF)
                {
                    token_list_free(&token_list);
                    return RC_SYN_ERR;
                }
                else if (ptr_last_token->token_type == TOKEN_RIGHT_BRACKET)
                {
                    token_list_add_item(&token_list, ptr_last_token);
                    switch_case = 5;
                    number_of_brackets--;
                }
                else if (ptr_last_token->token_type == TOKEN_PLUS)
                {
                    token_list_add_item(&token_list, ptr_last_token);
                    switch_case = 0;
                }
                else
                {
                    //TODO Evaluate the expression and generate the code here
                    token_listT token_postfix;
                    token_list_initialize(&token_postfix);
                    infix_to_postfix(&token_list, &token_postfix);

                    token_list_free(&token_list);
                    token_list_free(&token_postfix);
                    return 0;
                }
            }
            break;

            ///Case 3 = Number or identifier
            case 3:
            {
                ///Is it first token of the expression after identifier?
                if (is_it_first_token_of_expression != 0)
                {
                    ///Load the token
                    get_next_token(scanner, ptr_last_token, OPTIONAL);
                    ///We save the token to the structure of tokens
                    if (number_of_token > TOKEN_ARRAY_LEN)
                    {
                        token_list_free(&token_list);
                        return RC_RUN_ERR;
                    }
                }
                else
                {
                    ptr_last_token = ptr_start_token;
                    is_it_first_token_of_expression++;
                }

                ///What is the first token?
                if (ptr_last_token->token_type == RC_LEX_ERR)
                {
                    token_list_free(&token_list);
                    return RC_LEX_ERR;
                }
                else if (ptr_last_token->token_type == RC_RUN_ERR)
                {
                    token_list_free(&token_list);
                    return RC_RUN_ERR;
                }
                else if (ptr_last_token->token_type == TOKEN_RIGHT_BRACKET)
                {
                    token_list_add_item(&token_list, ptr_last_token);
                    switch_case = 5;
                    number_of_brackets--;
                }
                else if (ptr_last_token->token_type == TOKEN_EOF)
                {
                    token_list_free(&token_list);
                    return RC_SYN_ERR;
                }
                else if (operator_check(ptr_last_token))
                {
                    ///We have to check if the expr_data_type is not a string
                    if ((ptr_expr_data_and_type->token_type == EXPRESSION_NO_TYPE) || (ptr_expr_data_and_type->token_type == EXPRESSION_INT) || (ptr_expr_data_and_type->token_type == EXPRESSION_FLOAT64))
                    {
                        token_list_add_item(&token_list, ptr_last_token);
                        switch_case = 1;
                    }
                    else
                    {
                        token_list_free(&token_list);
                        return RC_SYN_ERR;
                    }
                }
                else if ((ptr_last_token->token_type == TOKEN_PLUS) || (ptr_last_token->token_type == TOKEN_DOUBLE_EQUAL) || (ptr_last_token->token_type == TOKEN_NOT_EQUAL))
                {
                    token_list_add_item(&token_list, ptr_last_token);
                    switch_case = 0;
                }
                else if (ptr_last_token->token_type == TOKEN_DIVISION)
                {
                    ///We have to check if the expr_data_type is a number
                    if ((ptr_expr_data_and_type->token_type == EXPRESSION_INT) || (ptr_expr_data_and_type->token_type == EXPRESSION_FLOAT64))
                    {
                        token_list_add_item(&token_list, ptr_last_token);
                        switch_case = 4;
                    }
                    else
                    {
                        token_list_free(&token_list);
                        return RC_SYN_ERR;
                    }
                }
                else
                {
                    //TODO Evaluate the expression and generate the code here
                    token_listT token_postfix;
                    token_list_initialize(&token_postfix);
                    infix_to_postfix(&token_list, &token_postfix);

                    token_list_free(&token_list);
                    token_list_free(&token_postfix);
                    return 0;
                }
            }
            break;

            ///Case 4 = Operator /
            case 4:
            {
                ///Load the token
                get_next_token(scanner, ptr_last_token, OPTIONAL);
                ///We save the token to the structure of tokens
                if (number_of_token > TOKEN_ARRAY_LEN)
                {
                    token_list_free(&token_list);
                    return RC_RUN_ERR;
                }
                token_list_add_item(&token_list, ptr_last_token);

                ///What is the next token?
                if (ptr_last_token->token_type == RC_LEX_ERR)
                {
                    token_list_free(&token_list);
                    return RC_LEX_ERR;
                }
                else if (ptr_last_token->token_type == RC_RUN_ERR)
                {
                    token_list_free(&token_list);
                    return RC_RUN_ERR;
                }
                else if (ptr_last_token->token_type == TOKEN_LEFT_BRACKET)
                {
                    switch_case = 0;
                    number_of_brackets++;
                }
                else if ((ptr_last_token->token_type == TOKEN_INTEGER_LITERAL) || (ptr_last_token->token_type == TOKEN_DECIMAL_LITERAL) || (ptr_last_token->token_type == TOKEN_EXPONENT_LITERAL))
                {
                    switch_case = 3;
                    ///If the number is an integer literal, its data type is an integer
                    ///If the number is a decimal literal or a number with an exponent, its data type is a float
                    if (ptr_last_token->token_type == TOKEN_INTEGER_LITERAL)
                    {
                        ///If expr_data_type variable contains nil or int
                        if ((ptr_expr_data_and_type->token_type == EXPRESSION_NO_TYPE) || (ptr_expr_data_and_type->token_type == EXPRESSION_INT))
                        {
                            ptr_expr_data_and_type->token_type = EXPRESSION_INT;
                        }
                        else
                        {
                            token_list_free(&token_list);
                            return RC_SEMANTIC_TYPE_COMPATIBILITY_ERR;
                        }

                        ///If the number is 0, it is division by the zero -> zero division error
                        ///The string can not be empty!
                        if (string_length(&ptr_last_token->attribute.string_val) != 0)
                        {
                            char first_symbol = string_first_character(&ptr_last_token->attribute.string_val);
                            if (first_symbol == '0')
                            {
                                token_list_free(&token_list);
                                return RC_ZERO_DIVISION_ERR;
                            }
                        }
                    }
                    else if ((ptr_last_token->token_type == TOKEN_DECIMAL_LITERAL) || (ptr_last_token->token_type == TOKEN_EXPONENT_LITERAL))
                    {
                        ///If expr_data_type contains nil or float
                        if ((ptr_expr_data_and_type->token_type == EXPRESSION_NO_TYPE) || (ptr_expr_data_and_type->token_type == EXPRESSION_FLOAT64))
                        {
                            ptr_expr_data_and_type->token_type = EXPRESSION_FLOAT64;
                        }
                        else
                        {
                            token_list_free(&token_list);
                            return RC_SEMANTIC_TYPE_COMPATIBILITY_ERR;
                        }
                    }
                    else
                    {
                        token_list_free(&token_list);
                        return RC_SEMANTIC_TYPE_COMPATIBILITY_ERR;
                    }
                }
                else if (ptr_last_token->token_type == TOKEN_IDENTIFIER)
                {
                    switch_case = 3;

                    semantic_symtable_symbol = stack_search(&scanner->st_stack, &ptr_last_token->attribute.string_val);
                    if (semantic_symtable_symbol == NULL)
                    {
                        token_list_free(&token_list);
                        return RC_SEMANTIC_IDENTIFIER_ERR;
                    }
                    ///If semantic_data_type contains int
                    if (semantic_symtable_symbol->type == TYPE_INT)
                    {
                        ///If expr_data_type is nil or int
                        if ((ptr_expr_data_and_type->token_type == EXPRESSION_NO_TYPE) || (ptr_expr_data_and_type->token_type == EXPRESSION_INT))
                        {
                            ptr_expr_data_and_type->token_type = EXPRESSION_INT;
                        }
                        else
                        {
                            token_list_free(&token_list);
                            return RC_SEMANTIC_TYPE_COMPATIBILITY_ERR;
                        }
                    }
                    ///If semantic_data_type contains float
                    else if (semantic_symtable_symbol->type == TYPE_DECIMAL)
                    {
                        ///If expr_data_type is nil or float
                        if ((ptr_expr_data_and_type->token_type == EXPRESSION_NO_TYPE) || (ptr_expr_data_and_type->token_type == EXPRESSION_FLOAT64))
                        {
                            ptr_expr_data_and_type->token_type = EXPRESSION_FLOAT64;
                        }
                        else
                        {
                            token_list_free(&token_list);
                            return RC_SEMANTIC_TYPE_COMPATIBILITY_ERR;
                        }
                    }
                    else
                    {
                        token_list_free(&token_list);
                        return RC_SEMANTIC_TYPE_COMPATIBILITY_ERR;
                    }
                }
                else
                {
                    token_list_free(&token_list);
                    return RC_SYN_ERR;
                }
            }
            break;

            ///Case 5 = Right bracket
            case 5:
            {
                ///Load the token
                get_next_token(scanner, ptr_last_token, OPTIONAL);
                ///We save the token to the structure of tokens
                if (number_of_token > TOKEN_ARRAY_LEN)
                {
                    token_list_free(&token_list);
                    return RC_RUN_ERR;
                }

                ///What is the first token?
                if (ptr_last_token->token_type == RC_LEX_ERR)
                {
                    token_list_free(&token_list);
                    return RC_LEX_ERR;
                }
                else if (ptr_last_token->token_type == RC_RUN_ERR)
                {
                    token_list_free(&token_list);
                    return RC_RUN_ERR;
                }
                else if (operator_check(ptr_last_token))
                {
                    ///We have to check if the expr_data_type is not a string
                    if ((ptr_expr_data_and_type->token_type == EXPRESSION_NO_TYPE) || (ptr_expr_data_and_type->token_type == EXPRESSION_INT) || (ptr_expr_data_and_type->token_type == EXPRESSION_FLOAT64))
                    {
                        token_list_add_item(&token_list, ptr_last_token);
                        switch_case = 1;
                    }
                    else
                    {
                        token_list_free(&token_list);
                        return RC_SEMANTIC_TYPE_COMPATIBILITY_ERR;
                    }
                }
                else if ((ptr_last_token->token_type == TOKEN_PLUS) || (ptr_last_token->token_type == TOKEN_DOUBLE_EQUAL) || (ptr_last_token->token_type == TOKEN_NOT_EQUAL))
                {
                    token_list_add_item(&token_list, ptr_last_token);
                    switch_case = 0;
                }
                else if (ptr_last_token->token_type == TOKEN_DIVISION)
                {
                    ///We have to check if the expr_data_type is not a string
                    if ((ptr_expr_data_and_type->token_type == EXPRESSION_NO_TYPE) || (ptr_expr_data_and_type->token_type == EXPRESSION_INT) || (ptr_expr_data_and_type->token_type == EXPRESSION_FLOAT64))
                    {
                        token_list_add_item(&token_list, ptr_last_token);
                        switch_case = 4;
                    }
                    else
                    {
                        token_list_free(&token_list);
                        return RC_SEMANTIC_TYPE_COMPATIBILITY_ERR;
                    }
                }
                else
                {
                    ///Now we must check if the number_of_brackets variable contains 0
                    if (number_of_brackets == 0)
                    {
                        //TODO Evaluate the expression and generate the code here
                        token_listT token_postfix;
                        token_list_initialize(&token_postfix);
                        infix_to_postfix(&token_list, &token_postfix);

                        token_list_free(&token_list);
                        token_list_free(&token_postfix);
                        return 0;
                    }
                    else
                    {
                        token_list_free(&token_list);
                        return RC_SYN_ERR;
                    }
                }
            }
            break;
        }
    }
}

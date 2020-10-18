//
// Created by Petr Vrtal on 13.10.2020.
//

#ifndef IFJ20_COMPILER_STRING_FUNCTIONS_H
#define IFJ20_COMPILER_STRING_FUNCTIONS_H

/**
 * Definitions of constants
 */
#define STRING_LENGTH           8

/**
 * Definitions of structure
 */
///Structure for saving the string
typedef struct
{
    ///String itself
    char *string;
    ///Length of the string
    int length;
    int alloc_size;
} stringT;

/**
 * Heads of functions
 */
///Function for the initialization (creation) of the new string
int string_init(stringT *str);
///Function for deleting of the string
void string_free(stringT *str);
///Function for deleting the content of the string
void string_clear(stringT *str);
///Function for clearing the string
void clear_str(stringT *str);
///Function can add the character to the end of the string
int string_add_character(stringT *str, char character);
///Function can add whole word to character
int string_add_string(stringT* str, char* word);
///Function for copying the string
int string_copy(stringT *first, stringT *second);
///Function for comparing two strings
int string_compare(stringT *first, stringT *second);
///Function for comparing the string and the constant string
int string_compare_constant(stringT *first, char *second);
///Function returns what the string contains
char *string_get(stringT *str);
///Function returns the length of the string
int string_length(stringT *str);
///Function returns first character of the string
///String can not be empty!!!
char string_first_character(stringT *str);

#endif //IFJ20_COMPILER_STRING_FUNCTIONS_H

//
// Created by Petr Vrtal on 13.10.2020.
//

#include <string.h>
#include <stdlib.h>
#include "string_functions.h"

// TODO: Reference old project that this file was copied from

/// Initialization (creation) of a new string
int string_init(stringT *str)
{
    ///If the memory allocation fail
    if ((str->string = (char*) malloc(STRING_LENGTH)) == NULL)
    {
        ///It is an error
        return 1;
    }
    ///The string is empty
    str->string[0] = '\0';
    str->length = 0;
    str->alloc_size = STRING_LENGTH;
    return 0;
}

///Function for deleting of the string
void string_free(stringT *str)
{
    free(str->string);
}

///Function for deleting the content of the string
void string_clear(stringT *str)
{
    for (int i = 0; i <= str->length; i++) {
        str->string[i] = '\0';
    }
    str->length = 0;
}
///Function for clearing the string
void clear_str(stringT *str){
    string_clear(str);
    string_free(str);
}

///Function can add the character to the end of the string
int string_add_character(stringT *str, char character)
{
    if (str->length + 1 >= str->alloc_size)
    {
        ///If we do not have enough of memory, we must realloc
        if ((str->string = (char*) realloc(str->string, str->length + STRING_LENGTH)) == NULL)
        {
            ///If memory allocation fail, it is an error
            return 1;
        }
        str->alloc_size = str->length + STRING_LENGTH;
    }
    ///Now we add character to the end of the string
    str->string[str->length] = character;
    ///We have to increase length of the string by one
    str->length = str->length + 1;
    str->string[str->length] = '\0';
    return 0;
}
//Function can add whole word to character
int string_add_string(stringT *str, char* word) {
    int length = strlen(word);
    int req_length = str->length + length + 1;
    if (req_length >= str->alloc_size) {
        if (!(str->string = (char *) realloc(str->string, req_length)))
            return 1;
        str->alloc_size = req_length;
    }
    str->length += length;
    strcat(str->string, word);
    str->string[str->length] = '\0';
    return 0;
}

///Function for copying the string
int string_copy(stringT *first, stringT *second)
{
    int new_length = second->length;
    if (new_length >= first->alloc_size)
    {
        ///If we do not have enough of memory, we must realloc
        if ((first->string = (char*) realloc(first->string, new_length + 1)) == NULL)
        {
            ///If memory allocation fail, it is an error
            return 1;
        }
        first->alloc_size = new_length + 1;
    }
    ///Now we call strcmp function for the copying second string to the first one
    strcpy(first->string, second->string);
    first->length = new_length;
    return 0;
}

///Function for comparing two strings
int string_compare(stringT *first, stringT *second)
{
    return strcmp(first->string, second->string);
}

///Function for comparing the string and the constant string
int string_compare_constant(stringT *first, char *second)
{
    return strcmp(first->string, second);
}

///Function returns what the string contains
char *string_get(stringT *str)
{
    return str->string;
}

///Function returns the length of the string
int string_length(stringT *str)
{
    return str->length;
}

///Function returns first character of the string
///String can not be empty!!!
char string_first_character(stringT *str)
{
    return str->string[0];
}

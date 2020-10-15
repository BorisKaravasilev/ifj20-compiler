//
// Created by Boris on 15-Oct-20.
//

#ifndef IFJ20_COMPILER_DEBUGGING_H
#define IFJ20_COMPILER_DEBUGGING_H

#include <stdio.h>

// ATTENTION
// __VA_ARGS__ can't be empty so you always have to provide some argument
// example workaround 'debug_scanner("String without any args%s", "")'
// %s is replaced by empty string so nothing changes, but macro works :)

// stdout or stderr (in CLion stderr order is random)
#define PRINT_TO stdout

// Prints to stderr debugging info if DEBUG is 'true'
// Macro by: Jonathan Leffler
// License: 'CC BY-SA 2.5.'
// from:
// https://stackoverflow.com/questions/1644868/define-macro-for-debug-printing-in-c
#define debug_scanner(fmt, ...)                               \
    do {                                              \
        if (DEBUG_SCANNER) fprintf(PRINT_TO, fmt, __VA_ARGS__); \
    } while (0)

#define debug_parser(fmt, ...)                               \
    do {                                              \
        if (DEBUG_PARSER) fprintf(PRINT_TO, fmt, __VA_ARGS__); \
    } while (0)

// Switches for debugging prints for separate parts (you can add more)
#define DEBUG_SCANNER true
#define DEBUG_PARSER true

#endif //IFJ20_COMPILER_DEBUGGING_H

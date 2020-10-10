//
// Created by Boris on 09-Oct-20.
//

#include <stdio.h>
#include <stdbool.h>

// Returns pointer to file or 'stdin'
FILE *get_input_file(bool enabled, char *file_name) {
    FILE *fp;

    if (enabled) {
        fp = fopen(file_name, "r");

        if (fp == NULL) {
            fprintf(stderr, "Input file doesn't exist\n");
        } else {
            return fp;
        }
    }

    return stdin;
}

#include "utility_functions.h"

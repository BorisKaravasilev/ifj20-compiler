cmake_minimum_required(VERSION 3.10)
project(ifj20_compiler)

set(CMAKE_CXX_STANDARD 14)

include_directories(general)
include_directories(input_files)
include_directories(parser)
include_directories(scanner)

add_executable(ifj20_compiler
        general/debugging.h
        general/return_codes.h
        general/string_functions.c
        general/string_functions.h
        general/utility_functions.c
        general/utility_functions.h
        parser/expression_analysis.c
        parser/expression_analysis.h
        parser/semantic_late_check.c
        parser/semantic_late_check.h
        parser/semantic_functions.c
        parser/semantic_functions.h
        scanner/finite_automata.c
        scanner/finite_automata.h
        scanner/scanner_functions.c
        scanner/scanner_functions.h
        scanner/stack.c
        scanner/stack.h
        scanner/states_list.h
        scanner/symtable.c
        scanner/symtable.h
        scanner/token_functions.c
        scanner/token_functions.h
        scanner/token_types.h
        ifj20-compiler.c)

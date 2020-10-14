CC=gcc
CFLAGS=-std=gnu99 -Wall -Wextra -pedantic
LFLAGS=-lpthread
BIN=ifj20-compiler
SOURCE=ifj20-compiler.c scanner/lex_functions.c scanner/finite_automata_definition.c general/utility_functions.c general/string_functions.c scanner/token_functions.c

all:
	$(CC) $(CFLAGS) $(SOURCE) -o $(BIN) $(LFLAGS)

run: all
	./$(BIN)

clean:
	rm $(BIN)

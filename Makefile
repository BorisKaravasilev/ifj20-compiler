CC=gcc
CFLAGS=-std=gnu99 -Wall -Wextra -pedantic
LFLAGS=-lpthread
BIN=ifj20-compiler
SOURCE_FOLDERS = $(wildcard general/*.c) \
                 $(wildcard generator/*.c) \
                 $(wildcard parser/*.c) \
                 $(wildcard scanner/*.c) \
                 $(wildcard *.c)
obj = $(SOURCE_FOLDERS:.c=.o)

compile_and_clean: all clean

all: $(obj)
	$(CC) $(CFLAGS) -o $(BIN) $^ $(LFLAGS)

run: all clean
	./$(BIN)

.PHONY: clean
clean:
	rm -f $(obj) myprog
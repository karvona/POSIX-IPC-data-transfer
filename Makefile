PROG = build/main
CC = gcc
CFLAGS = -Wall -g -std=c99 -D_POSIX_SOURCE -pedantic

all: $(PROG)

build/main: build/main.o build/morse.o build/parseio.o
	$(CC) $(CFLAGS) -o $@ $^


build/main.o: src/main.c src/morse.h src/parseio.h
	$(CC) $(CFLAGS) -c $< -o $@

build/morse.o: src/morse.c src/morse.h
	$(CC) $(CFLAGS) -c $< -o $@

build/parseio.o: src/parseio.c src/parseio.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(PROG) build/*.o *~

CC = clang
CFLAGS = -Wall -Wextra -pedantic

main: main.o scanner.o token.o string.o -lm
	$(CC) $(CFLAGS) $^ -o main

main.o: main.c scanner.c token.c string.c
	$(CC) $(CFLAGS) -c $^

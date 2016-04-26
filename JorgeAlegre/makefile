CC = gcc
CFLAGS = -Wall -Wextra -O3 -std=c99
OB = dirmain/color
SRC = apifiles/
BIN = bin/
MAIN = dirmain/

all:
	$(CC) $(CFLAGS) -Iapifiles $(MAIN)mainJAlegre.c $(SRC)*.c -o $(OB)

clean:
	rm $(OB)
	rm -rf $(OB).dSYM

try:
	$(OB)

valgrind:
	valgrind --show-reachable=yes --leak-check=full $(OB)

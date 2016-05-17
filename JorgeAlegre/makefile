CC = gcc
CFLAGS = -Wall -Wextra -O3 -std=c99 -g
OB = dirmain/color
SRC = apifiles/
BIN = bin/
MAIN = dirmain/

all:
	$(CC) $(CFLAGS) -Iapifiles $(MAIN)mainJAlegre.c $(SRC)*.c -o $(OB)

clean:
	rm $(OB)
	rm -rf $(OB).dSYM
	rm massif.*

try:
	time $(OB)

valgrind:
	time valgrind --show-reachable=yes --leak-check=full $(OB)

massif:
	valgrind --tool=massif --time-unit=B $(OB)

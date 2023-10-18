# Makefile

# setari ale compilatorului
CC = gcc
CFLAGS = -Wall -Wextra -g

# fisierul sursa
SRC = quadtree.c

# fisier obiect
OBJ = $(SRC:.c=.o)

# fisier executabil
EXEC = quadtree

# phony
.PHONY: all build run clean valgrind

all: build run

build: $(OBJ)
	$(CC) $(CFLAGS) -o $(EXEC) $(OBJ)

run:
	valgrind --leak-check=full --show-leak-kinds=all ./$(EXEC)

valgrind:
	valgrind ---leak-check=full --show-leak-kinds=all ./$(EXEC)

clean:
	rm -f $(OBJ) $(EXEC)


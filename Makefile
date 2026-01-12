CC=gcc
CFLAGS=-Wall -g
SRC=$(wildcard src/*.c)
OBJ=$(SRC:.c=.o)
EXEC=ProjetNetFlow

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f src/*.o $(EXEC)

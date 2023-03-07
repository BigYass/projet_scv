CC = gcc
CFLAGS = -Wall -Wextra -g

SRC = src
BIN = bin
OBJ = obj

SRCS = src/hash.c src/liste.c src/main.c
OBJS = $(SRCS:$(SRC)/%.c=$(OBJ)/%.o)
TARGET = main

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(BIN)/$@ $^

$(OBJ)/%.o: $(SRC)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(BIN)/* $(OBJ)/*.o

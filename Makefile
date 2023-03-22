CC = gcc
CFLAGS = -Wall -Wextra -ggdb

SRC = src
BIN = bin
OBJ = obj
TMP = .tmp

SRCS = src/hash.c src/liste.c src/main.c src/file.c src/test.c src/debug.c
OBJS = $(SRCS:$(SRC)/%.c=$(OBJ)/%.o)
TARGET = main

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(BIN)/$@ $^

$(OBJ)/%.o: $(SRC)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(BIN)/* $(OBJ)/*.o $(TMP)/*

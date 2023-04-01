CC = gcc
CFLAGS = -Wall -Wextra -ggdb

SRC = src
BIN = bin
OBJ = obj
TMP = .tmp

SRCS = $(wildcard $(SRC)/**/*.c) $(wildcard $(SRC)/*.c) 
OBJS = $(SRCS:$(SRC)/%.c=$(OBJ)/%.o)
TARGET = myGit

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(BIN)/$@ $^

$(OBJ)/%.o: $(SRC)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BIN)/* $(OBJ)/*.o $(TMP)/*

CC = gcc
CFLAGS = -Wall -Wextra -ggdb

# Grrr paw

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
	rm -rf $(BIN)/* $(OBJS)


run:
	make -s clean 
	make -s
	./$(BIN)/$(TARGET)
	make -s clean

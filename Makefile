CC = gcc
CFLAGS = -Wall -Wextra -g

SRCS = liste.c 
OBJS = $(SRCS:.c=.o)
TARGET = liste

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(OBJS) $(TARGET)

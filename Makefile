SRC = src/main.c
BIN = img2ascii
CC = gcc
CFLAGS = -Wall -Wextra
LIBS = -lm

all:
	$(CC) $(CFLAGS) -o $(BIN) $(SRC) $(LIBS)

clean:
	$(RM) $(BIN) $(BIN).exe
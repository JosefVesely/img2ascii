SRC=image_to_ascii.c
BIN=img2ascii
CC=gcc
CFLAGS=-Wall
LIBS=-lm

all:
	$(CC) $(CFLAGS) -o $(BIN) $(SRC) $(LIBS)

clean:
	$(RM) $(BIN) $(BIN).exe
SRC	= src/main.c src/args.c src/color.c src/utils.c src/error.c \
	src/ascii_art.c
BIN = img2ascii
CC = gcc
CFLAGS = -Wall -Wextra
LIBS = -lm -Iinclude -Istb_image

all:
	$(CC) $(CFLAGS) -o $(BIN) $(SRC) $(LIBS)

clean:
	$(RM) $(BIN) $(BIN).exe
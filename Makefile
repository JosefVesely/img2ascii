CC=gcc
CFLAGS=-Wall -lm

all:
	$(CC) $(CFLAGS) -o img2ascii image_to_ascii.c

clean:
	$(RM) img2ascii img2ascii.exe
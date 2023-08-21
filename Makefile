CC=gcc
CFLAGS=-Wall

all:
	$(CC) $(CFLAGS) -o img2ascii image_to_ascii.c -lm

clean:
	$(RM) img2ascii img2ascii.exe
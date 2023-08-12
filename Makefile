CC=gcc
CFLAGS=-Wall

all:
	$(CC) $(CFLAGS) -o img2ascii image_to_ascii.c

clean:
	rm img2ascii.exe
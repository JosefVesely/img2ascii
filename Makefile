CC=gcc
FLAGS=-Wall

image_to_ascii: image_to_ascii.c 
	$(CC) $(FLAGS) -o image_to_ascii image_to_ascii.c 
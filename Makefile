CFLAGS = -Wall -Werror -Wextra
CC = gcc
 
text_gen: text_gen.c hashtable.h
	$(CC) -g $(CFLAGS) -o text_gen text_gen.c hashtable.h


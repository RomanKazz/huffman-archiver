CC = gcc
CFLAGS = -Wall -Wextra -O2 -Iinclude

SRC = src/main.c src/huffman_tree.c src/huffman_codec.c src/bitio.c
OBJ = $(SRC:.c=.o)

huff: $(OBJ)
	$(CC) $(OBJ) -o huff

clean:
	rm -f *.o huff

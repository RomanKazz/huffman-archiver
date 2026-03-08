CC = gcc
CFLAGS = -Wall -Wextra -O2

SRC = main.c huffman_tree.c huffman_codec.c bitio.c
OBJ = $(SRC:.c=.o)

huff: $(OBJ)
	$(CC) $(OBJ) -o huff

clean:
	rm -f *.o huff

#include <stdio.h>

#include "huffman_codec.h"

int main(int argc, char* argv[]) {
    if (argc != 4) {
        printf("Usage:\n");
        printf("%s c input output\n", argv[0]);
        printf("%s d input output\n", argv[0]);
        return 1;
    }
    if (argv[1][0] == 'c') {
        return compress_file(argv[2], argv[3]);
    } else if (argv[1][0] == 'd') {
        return decompress_file(argv[2], argv[3]);
    } else {
        printf("Unknown mode\n");
        return 1;
    }

    return 0;
}

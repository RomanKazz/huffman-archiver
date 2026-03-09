#ifndef HUFFMAN_CODEC_H
#define HUFFMAN_CODEC_H

#include <stdio.h>

void write_header(FILE* out, int freq[256], int original_size);
int read_header(FILE* in, int freq[256], int* original_size);

void compress_file(const char* input, const char* output);
void decompress_file(const char* input, const char* output);

#endif  // HUFFMAN_CODEC_H

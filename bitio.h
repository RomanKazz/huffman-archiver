#ifndef BITIO_H
#define BITIO_H

#include <stdio.h>

typedef struct {
    FILE* file;
    unsigned char buffer;
    int bit_count;
} BitWriter;

typedef struct {
    FILE* file;
    unsigned char buffer;
    int bit_count;
} BitReader;

void write_bit(BitWriter* bw, int bit);
void write_code(BitWriter* bw, const char* code);
void flush_bits(BitWriter* bw);

int read_bit(BitReader* br);

#endif  // BITIO_H

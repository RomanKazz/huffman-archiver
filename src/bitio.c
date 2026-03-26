#include "bitio.h"

#include <stdlib.h>

struct BitWriter {
    FILE* file;
    unsigned char buffer;
    int bit_count;
};

struct BitReader {
    FILE* file;
    unsigned char buffer;
    int bit_count;
};

BitWriter* bw_create(FILE* file) {
    BitWriter* bw = malloc(sizeof(BitWriter));
    if (!bw) return NULL;

    bw->file = file;
    bw->buffer = 0;
    bw->bit_count = 0;
    return bw;
}

void bw_write_bit(BitWriter* bw, int bit) {
    bw->buffer <<= 1;
    bw->buffer |= bit;
    bw->bit_count++;

    if (bw->bit_count == 8) {
        fwrite(&bw->buffer, 1, 1, bw->file);
        bw->buffer = 0;
        bw->bit_count = 0;
    }
}

void bw_write_bits(BitWriter* bw, unsigned int code, int length) {
    for (int i = length - 1; i >= 0; i--) {
        bw_write_bit(bw, (code >> i) & 1);
    }
}

void bw_flush(BitWriter* bw) {
    if (bw->bit_count > 0) {
        bw->buffer <<= (8 - bw->bit_count);
        fwrite(&bw->buffer, 1, 1, bw->file);
    }
}

void bw_free(BitWriter* bw) { free(bw); }

BitReader* br_create(FILE* file) {
    BitReader* br = malloc(sizeof(BitReader));
    if (!br) return NULL;

    br->file = file;
    br->buffer = 0;
    br->bit_count = 0;
    return br;
}

int br_read_bit(BitReader* br) {
    if (br->bit_count == 0) {
        if (fread(&br->buffer, 1, 1, br->file) != 1) return -1;
        br->bit_count = 8;
    }

    int bit = (br->buffer >> 7) & 1;
    br->buffer <<= 1;
    br->bit_count--;

    return bit;
}

void br_free(BitReader* br) { free(br); }

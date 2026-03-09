#include "bitio.h"

void inline write_bit(BitWriter* bw, int bit) {
    bw->buffer <<= 1;
    bw->buffer |= bit;
    bw->bit_count++;

    if (bw->bit_count == 8) {
        fwrite(&bw->buffer, sizeof(unsigned char), 1, bw->file);
        bw->buffer = 0;
        bw->bit_count = 0;
    }
}

void write_code(BitWriter* bw, const char* code) {
    for (int i = 0; code[i]; i++) write_bit(bw, code[i] == '1');
}

void write_bits(BitWriter* bw, unsigned int code, int length) {
    for (int i = length - 1; i >= 0; i--) {
        write_bit(bw, (code >> i) & 1);
    }
}

void flush_bits(BitWriter* bw) {
    if (bw->bit_count > 0) {
        bw->buffer <<= (8 - bw->bit_count);
        fwrite(&bw->buffer, sizeof(unsigned char), 1, bw->file);
    }
}

int inline read_bit(BitReader* br) {
    if (br->bit_count == 0) {
        if(fread(&br->buffer, 1, 1, br->file) != 1) return -1;
        br->bit_count = 8;
    }

    int bit = (br->buffer >> 7) & 1;
    br->buffer <<= 1;
    br->bit_count--;

    return bit;
}

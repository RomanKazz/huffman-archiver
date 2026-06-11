// SPDX-License-Identifier: MIT
// Copyright (c) 2026 RomanKazz

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

BitWriter* bit_writer_create(FILE* file) {
    if (!file) return NULL;

    BitWriter* writer = malloc(sizeof(BitWriter));
    if (!writer) return NULL;

    writer->file = file;
    writer->buffer = 0;
    writer->bit_count = 0;
    return writer;
}

int bit_writer_write_bit(BitWriter* writer, int bit) {
    if (!writer) return 0;

    writer->buffer <<= 1;
    writer->buffer |= bit;
    writer->bit_count++;

    if (writer->bit_count == 8) {
        if (fwrite(&writer->buffer, 1, 1, writer->file) != 1) return 0;
        writer->buffer = 0;
        writer->bit_count = 0;
    }

    return 1;
}

int bit_writer_write_bits(BitWriter* writer, unsigned int code, int length) {
    for (int i = length - 1; i >= 0; i--) {
        if (!bit_writer_write_bit(writer, (code >> i) & 1)) return 0;
    }

    return 1;
}

int bit_writer_flush(BitWriter* writer) {
    if (!writer) return 0;

    if (writer->bit_count > 0) {
        writer->buffer <<= (8 - writer->bit_count);
        if (fwrite(&writer->buffer, 1, 1, writer->file) != 1) return 0;
        writer->buffer = 0;
        writer->bit_count = 0;
    }

    return 1;
}

void bit_writer_free(BitWriter* writer) { free(writer); }

BitReader* bit_reader_create(FILE* file) {
    if (!file) return NULL;

    BitReader* reader = malloc(sizeof(BitReader));
    if (!reader) return NULL;

    reader->file = file;
    reader->buffer = 0;
    reader->bit_count = 0;
    return reader;
}

int bit_reader_read_bit(BitReader* reader) {
    if (reader->bit_count == 0) {
        if (fread(&reader->buffer, 1, 1, reader->file) != 1) return -1;
        reader->bit_count = 8;
    }

    int bit = (reader->buffer >> 7) & 1;
    reader->buffer <<= 1;
    reader->bit_count--;

    return bit;
}

void bit_reader_free(BitReader* reader) { free(reader); }

// SPDX-License-Identifier: MIT
// Copyright (c) 2026 RomanKazz

#define _POSIX_C_SOURCE 200809L

#include "huffman_codec.h"

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>

#include "bitio.h"
#include "huffman_tree.h"

#define MAX_SYMBOLS 256
#define MAX_CODE_LEN 256

typedef struct {
    unsigned char bits[MAX_CODE_LEN];
    int length;
} HuffCode;

static int write_u64_le(FILE* out, uint64_t value) {
    unsigned char bytes[8];

    for (int i = 0; i < 8; i++) {
        bytes[i] = (unsigned char)((value >> (i * 8)) & 0xFFu);
    }

    return fwrite(bytes, 1, sizeof(bytes), out) == sizeof(bytes);
}

static int read_u64_le(FILE* in, uint64_t* value) {
    unsigned char bytes[8];

    if (fread(bytes, 1, sizeof(bytes), in) != sizeof(bytes)) {
        return 0;
    }

    *value = 0;
    for (int i = 0; i < 8; i++) {
        *value |= ((uint64_t)bytes[i]) << (i * 8);
    }

    return 1;
}

static int get_file_size(FILE* file, uint64_t* size) {
#if defined(_WIN32)
    __int64 end_offset;

    if (_fseeki64(file, 0, SEEK_END) != 0) {
        return 0;
    }

    end_offset = _ftelli64(file);
    if (end_offset < 0) {
        return 0;
    }
#else
    off_t end_offset;

    if (fseeko(file, 0, SEEK_END) != 0) {
        return 0;
    }

    end_offset = ftello(file);
    if (end_offset < 0) {
        return 0;
    }
#endif

    *size = (uint64_t)end_offset;
    rewind(file);
    return 1;
}

static int count_freq_file(FILE* file, uint64_t freq[MAX_SYMBOLS]) {
    unsigned char byte;

    while (fread(&byte, 1, 1, file) == 1) {
        freq[byte]++;
    }

    if (ferror(file)) {
        return 0;
    }

    rewind(file);
    return 1;
}

static void generate_codes(HuffmanNode* root, HuffCode table[MAX_SYMBOLS],
                           unsigned char path[MAX_CODE_LEN], int length) {
    if (!root) {
        return;
    }

    if (!root->left && !root->right) {
        if (length == 0) {
            table[root->ch].bits[0] = 0;
            table[root->ch].length = 1;
            return;
        }

        memcpy(table[root->ch].bits, path, (size_t)length);
        table[root->ch].length = length;
        return;
    }

    path[length] = 0;
    generate_codes(root->left, table, path, length + 1);

    path[length] = 1;
    generate_codes(root->right, table, path, length + 1);
}

static int write_code(BitWriter* writer, const HuffCode* code) {
    for (int i = 0; i < code->length; i++) {
        if (!bit_writer_write_bit(writer, code->bits[i])) {
            return 0;
        }
    }

    return 1;
}

int write_header(FILE* out, const uint64_t freq[MAX_SYMBOLS],
                 uint64_t original_size) {
    if (fwrite("HUFF", 1, 4, out) != 4) {
        return 0;
    }

    if (!write_u64_le(out, original_size)) {
        return 0;
    }

    for (int i = 0; i < MAX_SYMBOLS; i++) {
        if (!write_u64_le(out, freq[i])) {
            return 0;
        }
    }

    return 1;
}

int read_header(FILE* in, uint64_t freq[MAX_SYMBOLS], uint64_t* original_size) {
    char magic[4];

    if (fread(magic, 1, sizeof(magic), in) != sizeof(magic)) {
        return 0;
    }

    if (memcmp(magic, "HUFF", sizeof(magic)) != 0) {
        printf("Invalid archive format\n");
        return 0;
    }

    if (!read_u64_le(in, original_size)) {
        return 0;
    }

    for (int i = 0; i < MAX_SYMBOLS; i++) {
        if (!read_u64_le(in, &freq[i])) {
            return 0;
        }
    }

    return 1;
}

int compress_file(const char* input, const char* output) {
    int success = 0;
    uint64_t original_size = 0;
    uint64_t freq[MAX_SYMBOLS] = {0};
    HuffCode code_table[MAX_SYMBOLS] = {0};
    unsigned char path[MAX_CODE_LEN] = {0};
    FILE* in = fopen(input, "rb");
    FILE* out = NULL;
    HuffmanNode* root = NULL;
    BitWriter* writer = NULL;
    unsigned char byte;

    if (!in) {
        perror("file");
        return 0;
    }

    out = fopen(output, "wb");
    if (!out) {
        perror("file");
        fclose(in);
        return 0;
    }

    do {
        if (!count_freq_file(in, freq) || !get_file_size(in, &original_size)) {
            break;
        }

        if (!write_header(out, freq, original_size)) {
            break;
        }

        if (original_size == 0) {
            success = 1;
            break;
        }

        root = build_huffman_tree(freq);
        if (!root) {
            break;
        }

        generate_codes(root, code_table, path, 0);

        writer = bit_writer_create(out);
        if (!writer) {
            break;
        }

        int stream_ok = 1;
        while (fread(&byte, 1, 1, in) == 1) {
            if (!write_code(writer, &code_table[byte])) {
                stream_ok = 0;
                break;
            }
        }

        if (!stream_ok || ferror(in)) {
            break;
        }

        if (!bit_writer_flush(writer)) {
            break;
        }

        success = 1;
    } while (0);

    free_huffman_tree(root);
    bit_writer_free(writer);

    if (fclose(in) != 0) {
        success = 0;
    }
    if (fclose(out) != 0) {
        success = 0;
    }

    return success;
}

int decompress_file(const char* input, const char* output) {
    int success = 0;
    uint64_t freq[MAX_SYMBOLS] = {0};
    uint64_t original_size = 0;
    FILE* in = fopen(input, "rb");
    FILE* out = NULL;
    HuffmanNode* root = NULL;
    BitReader* reader = NULL;

    if (!in) {
        perror("file");
        return 0;
    }

    out = fopen(output, "wb");
    if (!out) {
        perror("file");
        fclose(in);
        return 0;
    }

    do {
        if (!read_header(in, freq, &original_size)) {
            printf("Failed to decompress file\n");
            break;
        }

        if (original_size == 0) {
            success = 1;
            break;
        }

        root = build_huffman_tree(freq);
        if (!root) {
            break;
        }

        reader = bit_reader_create(in);
        if (!reader) {
            break;
        }

        int decode_ok = 1;
        for (uint64_t i = 0; i < original_size; i++) {
            HuffmanNode* node = root;

            while (node->left || node->right) {
                int bit = bit_reader_read_bit(reader);
                if (bit == -1) {
                    decode_ok = 0;
                    break;
                }

                node = bit ? node->right : node->left;
            }

            if (node->left || node->right) {
                decode_ok = 0;
                break;
            }

            if (fputc(node->ch, out) == EOF) {
                decode_ok = 0;
                break;
            }
        }

        success = decode_ok && !ferror(out);
    } while (0);

    free_huffman_tree(root);
    bit_reader_free(reader);

    if (fclose(in) != 0) {
        success = 0;
    }
    if (fclose(out) != 0) {
        success = 0;
    }

    return success;
}

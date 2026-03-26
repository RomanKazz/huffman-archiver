#include "huffman_codec.h"

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "bitio.h"
#include "huffman_tree.h"

#define MAX 256
#define MAX_CODE_LEN 256

typedef struct {
    char ch;
    unsigned int code;
    int length;
    int freq;
} SymbolCode;

typedef struct {
    unsigned int code;
    int length;
} HuffCode;

uint64_t get_file_size(FILE* f) {
    fseek(f, 0, SEEK_END);
    uint64_t size = ftell(f);
    rewind(f);
    return size;
}

void count_freq_file(FILE* f, int freq[256]) {
    unsigned char byte;

    while (fread(&byte, 1, 1, f) == 1) freq[byte]++;

    rewind(f);
}

void build_code_table(SymbolCode codes[], int n, HuffCode table[256]) {
    for (int i = 0; i < n; i++) {
        unsigned char c = codes[i].ch;
        table[c].code = codes[i].code;
        table[c].length = codes[i].length;
    }
}

void write_header(FILE* out, int freq[MAX], uint64_t original_size) {
    fwrite("HUFF", sizeof(char), 4, out);
    fwrite(&original_size, sizeof(uint64_t), 1, out);
    fwrite(freq, sizeof(int), MAX, out);
}

int read_header(FILE* in, int freq[MAX], uint64_t* original_size) {
    char magic[4] = {0};

    if (fread(magic, 1, 4, in) != 4) return 0;

    if (memcmp(magic, "HUFF", 4) != 0) {
        printf("Invalid archive format\n");
        return 0;
    }

    if (fread(original_size, sizeof(uint64_t), 1, in) != 1) return 0;

    if (fread(freq, sizeof(int), MAX, in) != MAX) return 0;

    return 1;
}

void generate_codes(HuffmanNode* root, unsigned int code, int length,
                    SymbolCode* codes, int* index) {
    if (!root) return;

    if (!root->left && !root->right) {
        codes[*index].ch = root->ch;
        codes[*index].freq = root->freq;
        codes[*index].code = (length == 0 ? 0 : code);
        codes[*index].length = (length == 0 ? 1 : length);

        (*index)++;
        return;
    }

    generate_codes(root->left, code << 1, length + 1, codes, index);
    generate_codes(root->right, (code << 1) | 1, length + 1, codes, index);
}

void compress_file(const char* input, const char* output) {
    HuffCode code_table[256] = {0};

    FILE* in = fopen(input, "rb");
    FILE* out = fopen(output, "wb");

    if (!in || !out) {
        perror("file");
        if (in) fclose(in);
        if (out) fclose(out);
        return;
    }

    int freq[256] = {0};
    count_freq_file(in, freq);

    HuffmanNode* root = build_huffman_tree(freq);
    if (!root) {
        fclose(in);
        fclose(out);
        return;
    }

    uint64_t original_size = get_file_size(in);
    write_header(out, freq, original_size);

    SymbolCode codes[256];
    int index = 0;

    generate_codes(root, 0, 0, codes, &index);

    memset(code_table, 0, sizeof(code_table));
    build_code_table(codes, index, code_table);

    BitWriter* bw = bw_create(out);
    if (!bw) {
        fclose(in);
        fclose(out);
        return;
    }

    unsigned char byte;

    while (fread(&byte, 1, 1, in) == 1) {
        bw_write_bits(bw, code_table[byte].code, code_table[byte].length);
    }

    bw_flush(bw);

    fclose(in);
    fclose(out);

    free_huffman_tree(root);
    bw_free(bw);
}

void decompress_file(const char* input, const char* output) {
    FILE* in = fopen(input, "rb");
    FILE* out = fopen(output, "wb");

    if (!in || !out) {
        perror("file");
        if (in) fclose(in);
        if (out) fclose(out);
        return;
    }

    int freq[256];
    uint64_t original_size;

    if (read_header(in, freq, &original_size) == 0) {
        printf("Failed to decompress file\n");
        fclose(in);
        fclose(out);
        return;
    }

    HuffmanNode* root = build_huffman_tree(freq);
    if (!root) {
        fclose(in);
        fclose(out);
        return;
    }

    BitReader* br = br_create(in);
    if (!br) {
        fclose(in);
        fclose(out);
        free_huffman_tree(root);
        return;
    }

    for (uint64_t i = 0; i < original_size; i++) {
        HuffmanNode* node = root;

        while (node->left || node->right) {
            int bit = br_read_bit(br);
            if (bit == -1) {
                // corrupted archive
                fclose(in);
                fclose(out);
                free_huffman_tree(root);
                br_free(br);
                return;
            }

            if (bit)
                node = node->right;
            else
                node = node->left;
        }
        fputc(node->ch, out);
    }

    fclose(in);
    fclose(out);

    free_huffman_tree(root);
    br_free(br);
}

#include "huffman_codec.h"

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

HuffCode code_table[256];

long get_file_size(FILE* f) {
    fseek(f, 0, SEEK_END);
    long size = ftell(f);
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

void write_header(FILE* out, int freq[MAX], int original_size) {
    fwrite("HUFF", sizeof(char), 4, out);
    fwrite(&original_size, sizeof(int), 1, out);
    fwrite(freq, sizeof(int), MAX, out);
}

int read_header(FILE* in, int freq[MAX], int* original_size) {
    char magic[4];

    fread(magic, 1, 4, in);

    if (memcmp(magic, "HUFF", 4) != 0) {
        printf("Invalid archive format\n");
        fclose(in);
        return 0;
    }

    fread(original_size, sizeof(int), 1, in);
    fread(freq, sizeof(int), MAX, in);

    return 1;
}

void generateCodes(HuffmanNode* root, unsigned int code, int length,
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

    generateCodes(root->left, code << 1, length + 1, codes, index);
    generateCodes(root->right, (code << 1) | 1, length + 1, codes, index);
}

void compress_file(const char* input, const char* output) {
    FILE* in = fopen(input, "rb");
    FILE* out = fopen(output, "wb");

    if (!in || !out) {
        perror("file");
        return;
    }

    int freq[256] = {0};
    count_freq_file(in, freq);

    HuffmanNode* root = buildHuffmanTree(freq);

    int original_size = get_file_size(in);
    write_header(out, freq, original_size);

    if (!root) {
        fclose(in);
        fclose(out);
        return;
    }

    SymbolCode codes[256];
    int index = 0;

    generateCodes(root, 0, 0, codes, &index);

    memset(code_table, 0, sizeof(code_table));
    build_code_table(codes, index, code_table);

    BitWriter bw;
    bw.file = out;
    bw.bit_count = 0;
    bw.buffer = 0;

    unsigned char byte;

    while (fread(&byte, 1, 1, in) == 1) {
        write_bits(&bw, code_table[byte].code, code_table[byte].length);
    }

    flush_bits(&bw);

    fclose(in);
    fclose(out);

    freeHuffmanTree(root);
}

void decompress_file(const char* input, const char* output) {
    FILE* in = fopen(input, "rb");
    FILE* out = fopen(output, "wb");

    if (!in || !out) {
        perror("file");
        return;
    }

    int freq[256];
    int original_size;

    if(read_header(in, freq, &original_size) == 0) {
        printf("Failed to decompress file\n");
        fclose(in);
        fclose(out);
        return;
    }

    HuffmanNode* root = buildHuffmanTree(freq);

    BitReader br;
    br.file = in;
    br.bit_count = 0;
    br.buffer = 0;

    for (int i = 0; i < original_size; i++) {
        HuffmanNode* node = root;

        while (node->left || node->right) {
            int bit = read_bit(&br);

            if (bit)
                node = node->right;
            else
                node = node->left;
        }
        fputc(node->ch, out);
    }

    fclose(in);
    fclose(out);

    freeHuffmanTree(root);
}

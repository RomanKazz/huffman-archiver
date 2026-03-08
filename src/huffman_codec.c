#include "huffman_codec.h"

#include <stdio.h>
#include <string.h>

#include "bitio.h"
#include "huffman_tree.h"

#define MAX 256
#define MAX_CODE_LEN 256

typedef struct {
    char ch;
    char code[MAX_CODE_LEN];
    int freq;
} SymbolCode;

char code_table[256][MAX_CODE_LEN];

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

void build_code_table(SymbolCode codes[], int n,
                      char table[256][MAX_CODE_LEN]) {
    for (int i = 0; i < n; i++)
        strcpy(table[(unsigned char)codes[i].ch], codes[i].code);
}

void write_header(FILE* out, int freq[MAX], int original_size) {
    fwrite("HUFF", sizeof(char), 4, out);
    fwrite(&original_size, sizeof(int), 1, out);
    fwrite(freq, sizeof(int), MAX, out);
}

void read_header(FILE* in, int freq[MAX], int* original_size) {
    char magic[4];

    fread(magic, 1, 4, in);

    if (memcmp(magic, "HUFF", 4) != 0) {
        printf("Invalid archive format\n");
        fclose(in);
        return;
    }

    fread(original_size, sizeof(int), 1, in);
    fread(freq, sizeof(int), MAX, in);
}

void generateCodes(HuffmanNode* root, char* code, int depth, SymbolCode* codes,
                   int* index) {
    if (!root) return;
    if (!root->left && !root->right) {
        codes[*index].ch = root->ch;
        codes[*index].freq = root->freq;

        if (depth == 0) {
            code[0] = '0';
            code[1] = '\0';
        } else {
            code[depth] = '\0';
        }

        strcpy(codes[*index].code, code);

        (*index)++;
        return;
    }
    if (root->left) {
        code[depth] = '0';
        generateCodes(root->left, code, depth + 1, codes, index);
    }
    if (root->right) {
        code[depth] = '1';
        generateCodes(root->right, code, depth + 1, codes, index);
    }
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

    if (!root) {
        printf("Input file is empty\n");
        fclose(in);
        fclose(out);
        return;
    }

    SymbolCode codes[256];
    char code[MAX_CODE_LEN];
    int index = 0;

    generateCodes(root, code, 0, codes, &index);

    memset(code_table, 0, sizeof(code_table));
    build_code_table(codes, index, code_table);

    int original_size = get_file_size(in);

    write_header(out, freq, original_size);

    BitWriter bw;
    bw.file = out;
    bw.bit_count = 0;
    bw.buffer = 0;

    unsigned char byte;

    while (fread(&byte, 1, 1, in) == 1) {
        write_code(&bw, code_table[byte]);
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

    read_header(in, freq, &original_size);

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

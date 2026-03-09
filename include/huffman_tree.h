#ifndef HUFFMAN_TREE_H
#define HUFFMAN_TREE_H

typedef struct HuffmanNode {
    unsigned char ch;
    int freq;
    struct HuffmanNode* left;
    struct HuffmanNode* right;
} HuffmanNode;

HuffmanNode* buildHuffmanTree(int freq[]);
void freeHuffmanTree(HuffmanNode* root);

#endif  // HUFFMAN_TREE_H

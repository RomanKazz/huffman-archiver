#include "huffman_tree.h"

#include <stdio.h>
#include <stdlib.h>

#define MAX 256

typedef struct {
    int size;
    HuffmanNode* items[MAX];
} PriorityQueue;

HuffmanNode* createNode(char ch, int freq) {
    HuffmanNode* node = (HuffmanNode*)malloc(sizeof(HuffmanNode));
    node->ch = ch;
    node->freq = freq;
    node->left = node->right = NULL;
    return node;
}

void swapNodes(HuffmanNode** a, HuffmanNode** b) {
    HuffmanNode* temp = *a;
    *a = *b;
    *b = temp;
}

void heapifyUp(PriorityQueue* pq, int index) {
    if (index && pq->items[(index - 1) / 2]->freq > pq->items[index]->freq) {
        swapNodes(&pq->items[(index - 1) / 2], &pq->items[index]);
        heapifyUp(pq, (index - 1) / 2);
    }
}

void heapifyDown(PriorityQueue* pq, int index) {
    int smallest = index;
    int left = 2 * index + 1;
    int right = 2 * index + 2;

    if (left < pq->size && pq->items[left]->freq < pq->items[smallest]->freq)
        smallest = left;

    if (right < pq->size && pq->items[right]->freq < pq->items[smallest]->freq)
        smallest = right;

    if (smallest != index) {
        swapNodes(&pq->items[index], &pq->items[smallest]);
        heapifyDown(pq, smallest);
    }
}

void enqueue(PriorityQueue* pq, HuffmanNode* node) {
    if (pq->size == MAX) {
        printf("Priority queue is full.\n");
        return;
    }

    pq->items[pq->size++] = node;
    heapifyUp(pq, pq->size - 1);
}

HuffmanNode* dequeue(PriorityQueue* pq) {
    if (!pq->size) {
        return NULL;
    }

    HuffmanNode* node = pq->items[0];
    pq->items[0] = pq->items[--pq->size];
    heapifyDown(pq, 0);
    return node;
}

HuffmanNode* buildHuffmanTree(int freq[]) {
    PriorityQueue pq;
    pq.size = 0;

    for (int i = 0; i < 256; i++)
        if (freq[i] > 0) enqueue(&pq, createNode((char)i, freq[i]));

    while (pq.size > 1) {
        HuffmanNode* left = dequeue(&pq);
        HuffmanNode* right = dequeue(&pq);

        HuffmanNode* internal = createNode('\0', left->freq + right->freq);
        internal->left = left;
        internal->right = right;

        enqueue(&pq, internal);
    }

    return dequeue(&pq);
}

void freeHuffmanTree(HuffmanNode* root) {
    if (!root) return;
    freeHuffmanTree(root->left);
    freeHuffmanTree(root->right);
    free(root);
}

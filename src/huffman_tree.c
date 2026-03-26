#include "huffman_tree.h"

#include <stdio.h>
#include <stdlib.h>

#define MAX 256

typedef struct {
    int size;
    HuffmanNode* items[MAX];
} PriorityQueue;

HuffmanNode* create_node(char ch, int freq) {
    HuffmanNode* node = (HuffmanNode*)malloc(sizeof(HuffmanNode));
    if (!node) return NULL;
    node->ch = ch;
    node->freq = freq;
    node->left = node->right = NULL;
    return node;
}

void swap_nodes(HuffmanNode** a, HuffmanNode** b) {
    HuffmanNode* temp = *a;
    *a = *b;
    *b = temp;
}

void heapify_up(PriorityQueue* pq, int index) {
    if (index && pq->items[(index - 1) / 2]->freq > pq->items[index]->freq) {
        swap_nodes(&pq->items[(index - 1) / 2], &pq->items[index]);
        heapify_up(pq, (index - 1) / 2);
    }
}

void heapify_down(PriorityQueue* pq, int index) {
    int smallest = index;
    int left = 2 * index + 1;
    int right = 2 * index + 2;

    if (left < pq->size && pq->items[left]->freq < pq->items[smallest]->freq)
        smallest = left;

    if (right < pq->size && pq->items[right]->freq < pq->items[smallest]->freq)
        smallest = right;

    if (smallest != index) {
        swap_nodes(&pq->items[index], &pq->items[smallest]);
        heapify_down(pq, smallest);
    }
}

void enqueue(PriorityQueue* pq, HuffmanNode* node) {
    if (pq->size == MAX) {
        printf("Priority queue is full.\n");
        return;
    }

    pq->items[pq->size++] = node;
    heapify_up(pq, pq->size - 1);
}

HuffmanNode* dequeue(PriorityQueue* pq) {
    if (!pq->size) {
        return NULL;
    }

    HuffmanNode* node = pq->items[0];
    pq->items[0] = pq->items[--pq->size];
    heapify_down(pq, 0);
    return node;
}

HuffmanNode* build_huffman_tree(int freq[]) {
    PriorityQueue pq;
    pq.size = 0;

    for (int i = 0; i < 256; i++) {
        if (freq[i] > 0) {
            HuffmanNode* node = create_node((char)i, freq[i]);
            if (!node) return NULL;

            enqueue(&pq, node);
        }
    }

    while (pq.size > 1) {
        HuffmanNode* left = dequeue(&pq);
        HuffmanNode* right = dequeue(&pq);

        HuffmanNode* internal = create_node('\0', left->freq + right->freq);
        if (!internal) return NULL;
        internal->left = left;
        internal->right = right;

        enqueue(&pq, internal);
    }

    return dequeue(&pq);
}

void free_huffman_tree(HuffmanNode* root) {
    if (!root) return;
    free_huffman_tree(root->left);
    free_huffman_tree(root->right);
    free(root);
}

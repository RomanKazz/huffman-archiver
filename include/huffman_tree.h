#ifndef HUFFMAN_TREE_H
#define HUFFMAN_TREE_H

/**
 * Узел дерева Хаффмана.
 *
 * Хранит символ, его частоту и ссылки на дочерние узлы.
 * Листовые узлы соответствуют символам исходного файла.
 */
typedef struct HuffmanNode {
    unsigned char ch;           // символ
    int freq;                   // частота появления
    struct HuffmanNode* left;   // левый потомок (бит 0)
    struct HuffmanNode* right;  // правый потомок (бит 1)
} HuffmanNode;

/**
 * Строит дерево Хаффмана по массиву частот символов.
 *
 * @param freq массив частот длины 256 (для всех возможных байтов)
 * @return указатель на корень дерева или NULL, если вход пуст
 */
HuffmanNode* build_huffman_tree(int freq[]);

/**
 * Освобождает память, занятую деревом Хаффмана.
 *
 * @param root корень дерева
 */
void free_huffman_tree(HuffmanNode* root);

#endif  // HUFFMAN_TREE_H

#include <stdlib.h>

#include "huffman_tree.h"
#include "test.h"

TEST(tree_basic) {
    uint64_t freq[256] = {0};

    freq['a'] = 5;
    freq['b'] = 2;

    HuffmanNode* root = build_huffman_tree(freq);

    EXPECT_TRUE(root != NULL);
    EXPECT_EQ(root->freq, 7);

    free_huffman_tree(root);
}

TEST(tree_single_symbol) {
    uint64_t freq[256] = {0};
    freq['a'] = 10;

    HuffmanNode* root = build_huffman_tree(freq);

    EXPECT_TRUE(root != NULL);
    EXPECT_EQ(root->ch, 'a');
    EXPECT_EQ(root->freq, 10);
    EXPECT_TRUE(root->right == NULL);
    EXPECT_TRUE(root->left == NULL);

    free_huffman_tree(root);
}

TEST(tree_empty) {
    uint64_t freq[256] = {0};

    HuffmanNode* root = build_huffman_tree(freq);

    EXPECT_TRUE(root == NULL);
}

TEST(tree_frequency_sum) {
    uint64_t freq[256] = {0};

    freq['a'] = 5;
    freq['b'] = 3;
    freq['c'] = 2;

    HuffmanNode* root = build_huffman_tree(freq);

    EXPECT_EQ(root->freq, 10);

    free_huffman_tree(root);
}

int main() {
    RUN_TEST(tree_basic);
    RUN_TEST(tree_single_symbol);
    RUN_TEST(tree_empty);
    RUN_TEST(tree_frequency_sum);

    TEST_REPORT();
}

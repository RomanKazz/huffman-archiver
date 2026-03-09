#include "huffman_tree.h"
#include "test.h"

TEST(tree_basic)
{
    int freq[256] = {0};

    freq['a'] = 5;
    freq['b'] = 2;

    HuffmanNode* root = buildHuffmanTree(freq);

    EXPECT_TRUE(root != NULL);
    EXPECT_EQ(root->freq, 7);

    freeHuffmanTree(root);
}

TEST(tree_single_symbol)
{
    int freq[256] = {0};
    freq['a'] = 10;

    HuffmanNode* root = buildHuffmanTree(freq);

    EXPECT_TRUE(root != NULL);
    EXPECT_EQ(root->ch, 'a');
    EXPECT_EQ(root->freq, 10);
    EXPECT_TRUE(root->right == NULL);
    EXPECT_TRUE(root->left == NULL);

    freeHuffmanTree(root);
}

TEST(tree_empty)
{
    int freq[256] = {0};

    HuffmanNode* root = buildHuffmanTree(freq);

    EXPECT_TRUE(root == NULL);
}

TEST(tree_frequency_sum)
{
    int freq[256] = {0};

    freq['a'] = 5;
    freq['b'] = 3;
    freq['c'] = 2;

    HuffmanNode* root = buildHuffmanTree(freq);

    EXPECT_EQ(root->freq, 10);

    freeHuffmanTree(root);
}

int main()
{
    RUN_TEST(tree_basic);
    RUN_TEST(tree_single_symbol);
    RUN_TEST(tree_empty);
    RUN_TEST(tree_frequency_sum);

    TEST_REPORT();
}

#include <stdio.h>
#include <string.h>

#include "huffman_codec.h"
#include "test.h"

TEST(header_rw)
{
    const char* file = TO_TEST("header.bin");

    FILE* f = fopen(file, "wb");

    int freq[256] = {0};
    freq['a'] = 10;
    freq['b'] = 5;

    write_header(f, freq, 1234);
    fclose(f);

    f = fopen(file, "rb");

    int freq2[256];
    int size;

    EXPECT_TRUE(read_header(f, freq2, &size));

    EXPECT_EQ(size, 1234);
    EXPECT_EQ(freq2['a'], 10);
    EXPECT_EQ(freq2['b'], 5);

    fclose(f);
    remove(file);
}

int main()
{
    RUN_TEST(header_rw);
    TEST_REPORT();
}

#include <stdio.h>

#include "huffman_codec.h"
#include "test.h"

TEST(corrupted_archive) {
    const char* file = TO_TEST("bad.huff");

    FILE* f = fopen(file, "wb");
    EXPECT_TRUE(f != NULL);

    for(int i = 0; i < 100; i++) fputc(i, f);

    fclose(f);

    decompress_file(file, TO_TEST("out.txt"));
    remove(TO_TEST("out.txt"));
}

int main() {
    RUN_TEST(corrupted_archive);
    TEST_REPORT();
}

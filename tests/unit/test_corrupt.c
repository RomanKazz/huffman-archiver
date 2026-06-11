// SPDX-License-Identifier: MIT
// Copyright (c) 2026 RomanKazz

#include <stdio.h>

#include "huffman_codec.h"
#include "test.h"

TEST(corrupted_archive) {
    const char* file = TO_TEST("bad.huff");

    EXPECT_TRUE(ensure_test_tmp_dir());
    FILE* f = fopen(file, "wb");
    EXPECT_TRUE(f != NULL);

    for (int i = 0; i < 100; i++) fputc(i, f);

    fclose(f);

    EXPECT_EQ(decompress_file(file, TO_TEST("out.txt")), 0);
    remove(TO_TEST("out.txt"));
}

int main() {
    RUN_TEST(corrupted_archive);
    TEST_REPORT();
}

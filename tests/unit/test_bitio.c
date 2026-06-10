// SPDX-License-Identifier: MIT
// Copyright (c) 2026 RomanKazz

#include <stdio.h>

#include "bitio.h"
#include "test.h"

TEST(write_read_bits) {
    const char* tmp = TO_TEST("tmp.bin");

    EXPECT_TRUE(ensure_test_tmp_dir());
    FILE* f = fopen(tmp, "wb");
    EXPECT_TRUE(f != NULL);

    BitWriter* writer = bit_writer_create(f);
    EXPECT_TRUE(writer != NULL);

    for (int i = 0; i < 100; i++) bit_writer_write_bit(writer, i % 2);

    bit_writer_flush(writer);
    bit_writer_free(writer);
    fclose(f);

    f = fopen(TO_TEST("tmp.bin"), "rb");
    EXPECT_TRUE(f != NULL);

    BitReader* reader = bit_reader_create(f);
    EXPECT_TRUE(reader != NULL);

    for (int i = 0; i < 100; i++) EXPECT_EQ(bit_reader_read_bit(reader), i % 2);

    bit_reader_free(reader);

    fclose(f);
    remove(tmp);
}

int main() {
    RUN_TEST(write_read_bits);
    TEST_REPORT();
}

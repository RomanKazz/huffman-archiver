#include <stdio.h>

#include "bitio.h"
#include "test.h"

TEST(write_read_bits) {
    const char* tmp = TO_TEST("tmp.bin");

    EXPECT_TRUE(ensure_test_tmp_dir());
    FILE* f = fopen(tmp, "wb");
    EXPECT_TRUE(f != NULL);

    BitWriter* bw = bw_create(f);
    EXPECT_TRUE(bw != NULL);

    for (int i = 0; i < 100; i++) bw_write_bit(bw, i % 2);

    bw_flush(bw);
    bw_free(bw);
    fclose(f);

    f = fopen(TO_TEST("tmp.bin"), "rb");
    EXPECT_TRUE(f != NULL);

    BitReader* br = br_create(f);
    EXPECT_TRUE(br != NULL);

    for (int i = 0; i < 100; i++) EXPECT_EQ(br_read_bit(br), i % 2);

    br_free(br);

    fclose(f);
    remove(tmp);
}

int main() {
    RUN_TEST(write_read_bits);
    TEST_REPORT();
}

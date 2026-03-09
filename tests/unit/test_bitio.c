#include <stdio.h>

#include "bitio.h"
#include "test.h"

TEST(write_read_bits) {
    const char* tmp = TO_TEST("tmp.bin");

    FILE* f = fopen(tmp, "wb");
    EXPECT_TRUE(f != NULL);

    BitWriter bw = {f, 0, 0};

    for (int i = 0; i < 100; i++) write_bit(&bw, i % 2);

    flush_bits(&bw);
    fclose(f);

    f = fopen(TO_TEST("tmp.bin"), "rb");
    EXPECT_TRUE(f != NULL);

    BitReader br = {f, 0, 0};

    for (int i = 0; i < 100; i++) EXPECT_EQ(read_bit(&br), i % 2);

    fclose(f);
    remove(tmp);
}

int main() {
    RUN_TEST(write_read_bits);
    TEST_REPORT();
}

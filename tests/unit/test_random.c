#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "huffman_codec.h"
#include "test.h"

TEST(random_archive) {
    const char* in = TO_TEST("random_in.bin");
    const char* arc = TO_TEST("random.huff");
    const char* out = TO_TEST("random_out.bin");

    FILE* f = fopen(in, "wb");
    EXPECT_TRUE(f != NULL);

    srand(time(0));

    for (int i = 0; i < 10000; i++) fputc(rand() % 256, f);

    fclose(f);

    compress_file(in, arc);
    decompress_file(arc, out);

    EXPECT_TRUE(files_equal(in, out));

    remove(in);
    remove(arc);
    remove(out);
}

TEST(stress_random) {
    for(int i = 0; i < 100; i++)
        random_archive();
}

int main() {
    RUN_TEST(random_archive);
    RUN_TEST(stress_random);
    TEST_REPORT();
}

#include <stdio.h>

#include "huffman_codec.h"
#include "test.h"

TEST(codec_basic) { TEST_ARCHIVE("Hello, world!"); }

TEST(codec_repeated) { TEST_ARCHIVE("aaaaaaaaaaaaaaaaaaaaaa"); }

TEST(codec_sentence) {
    TEST_ARCHIVE(
        "Lorem ipsum dolor sit amet consectetur adipiscing elit. Quisque "
        "faucibus ex sapien vitae pellentesque sem placerat.");
}

TEST(codec_empty_file) {
    const char* in = TO_TEST("empty_input.bin");
    const char* arc = TO_TEST("empty_archive.huff");
    const char* out = TO_TEST("empty_output.bin");

    EXPECT_TRUE(ensure_test_tmp_dir());

    FILE* file = fopen(in, "wb");
    EXPECT_TRUE(file != NULL);
    fclose(file);

    EXPECT_EQ(compress_file(in, arc), 0);
    EXPECT_EQ(decompress_file(arc, out), 0);
    EXPECT_TRUE(files_equal(in, out));

    remove(in);
    remove(arc);
    remove(out);
}

TEST(codec_long_codes) {
    const char* in = TO_TEST("fib_input.bin");
    const char* arc = TO_TEST("fib_archive.huff");
    const char* out = TO_TEST("fib_output.bin");
    uint64_t freq[34] = {1, 1};

    for (int i = 2; i < 34; i++) {
        freq[i] = freq[i - 1] + freq[i - 2];
    }

    EXPECT_TRUE(ensure_test_tmp_dir());

    FILE* file = fopen(in, "wb");
    EXPECT_TRUE(file != NULL);

    for (int symbol = 0; symbol < 34; symbol++) {
        for (uint64_t count = 0; count < freq[symbol]; count++) {
            fputc(symbol, file);
        }
    }
    fclose(file);

    EXPECT_EQ(compress_file(in, arc), 0);
    EXPECT_EQ(decompress_file(arc, out), 0);
    EXPECT_TRUE(files_equal(in, out));

    remove(in);
    remove(arc);
    remove(out);
}

int main() {
    RUN_TEST(codec_basic);
    RUN_TEST(codec_repeated);
    RUN_TEST(codec_sentence);
    RUN_TEST(codec_empty_file);
    RUN_TEST(codec_long_codes);
    TEST_REPORT();
}

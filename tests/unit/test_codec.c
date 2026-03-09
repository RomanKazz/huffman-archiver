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

int main() {
    RUN_TEST(codec_basic);
    RUN_TEST(codec_repeated);
    RUN_TEST(codec_sentence);
    TEST_REPORT();
}

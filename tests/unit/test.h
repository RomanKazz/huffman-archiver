#ifndef TEST_H
#define TEST_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "huffman_codec.h"

#define RED "\033[31m"
#define GREEN "\033[32m"
#define RESET "\033[0m"

static int tests_run = 0;
static int tests_failed = 0;

static inline int files_equal(const char* a, const char* b) {
    FILE* fa = fopen(a, "rb");
    FILE* fb = fopen(b, "rb");

    if (!fa || !fb) return 0;

    int ca, cb;

    while (1) {
        ca = fgetc(fa);
        cb = fgetc(fb);

        if (ca != cb) {
            fclose(fa);
            fclose(fb);
            return 0;
        }

        if (ca == EOF) break;
    }

    fclose(fa);
    fclose(fb);

    return 1;
}

#define EXPECT_TRUE(cond)                                          \
    do {                                                           \
        if (!(cond)) {                                             \
            printf(RED "FAIL: %s:%d\n" RESET, __FILE__, __LINE__); \
            tests_failed++;                                        \
        }                                                          \
    } while (0)

#define EXPECT_EQ(a, b)                                            \
    do {                                                           \
        if ((a) != (b)) {                                          \
            printf(RED "FAIL: %s:%d\n" RESET, __FILE__, __LINE__); \
            tests_failed++;                                        \
        }                                                          \
    } while (0)

#define TEST_ARCHIVE(text)                         \
    do {                                           \
        const char* in = "tests/tmp/input.txt";    \
        const char* arc = "tests/tmp/archive.txt"; \
        const char* out = "tests/tmp/out.txt";     \
                                                   \
        FILE* f = fopen(in, "wb");                 \
        if (!f) {                                  \
            perror("fopen");                       \
            exit(1);                               \
        }                                          \
        fprintf(f, "%s", text);                    \
        fclose(f);                                 \
                                                   \
        compress_file(in, arc);                    \
        decompress_file(arc, out);                 \
                                                   \
        EXPECT_TRUE(files_equal(in, out));         \
                                                   \
        remove(in);                                \
        remove(arc);                               \
        remove(out);                               \
    } while (0)

#define TEST(name) void name()

#define RUN_TEST(test)                    \
    do {                                  \
        printf("Running %s...\n", #test); \
        tests_run++;                      \
        test();                           \
    } while (0)

#define TEST_REPORT()                             \
    do {                                          \
        printf("\nTests run: %d\n", tests_run);   \
        printf("Failures: %d\n\n", tests_failed); \
        if (tests_failed) exit(1);                \
    } while (0)

#define TO_TEST(name) "tests/tmp/" name

#endif  // TEST_H

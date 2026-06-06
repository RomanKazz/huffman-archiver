#!/bin/bash
set -e

BUILD_DIR="${BUILD_DIR:-build-tests}"
ARCHIVER="$BUILD_DIR/huff"
ARCHIVER_PREFIX="${ARCHIVER_PREFIX:-}"
TMP_DIR=tests/tmp
TMP_ARCHIVE="$TMP_DIR/tmp.huff"
TMP_OUTPUT="$TMP_DIR/tmp.out"

passed=0
failed=0

GREEN="\033[0;32m"
RED="\033[0;31m"
RESET="\033[0m"

run_test() {
    input=$1
    echo "Test: $input"

    $ARCHIVER_PREFIX "$ARCHIVER" c "$input" "$TMP_ARCHIVE"
    $ARCHIVER_PREFIX "$ARCHIVER" d "$TMP_ARCHIVE" "$TMP_OUTPUT"

    if diff "$input" "$TMP_OUTPUT" > /dev/null; then
        echo -e "${GREEN}PASS${RESET}"
        passed=$((passed+1))
    else
        echo -e "${RED}FAILED${RESET}"
        failed=$((failed+1))
    fi

    echo
}

echo "Running Huffman tests..."
echo

mkdir -p "$TMP_DIR"

cmake -S . -B "$BUILD_DIR" ${CMAKE_CONFIGURE_ARGS:-}
cmake --build "$BUILD_DIR"

for file in tests/data/*.txt tests/data/*.bin; do
    run_test "$file"
done

echo "=================="
echo "Passed: $passed"
echo "Failed: $failed"

rm -f "$TMP_ARCHIVE" "$TMP_OUTPUT"

if [ $failed -ne 0 ]; then
    exit 1
fi

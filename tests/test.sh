#!/bin/bash
set -e

ARCHIVER=build/huff
TMP_ARCHIVE=tmp.huff
TMP_OUTPUT=tmp.out

passed=0
failed=0

GREEN="\033[0;32m"
RED="\033[0;31m"
RESET="\033[0m"

run_test() {
    input=$1
    echo "Test: $input"

    $ARCHIVER c "$input" "$TMP_ARCHIVE"
    $ARCHIVER d "$TMP_ARCHIVE" "$TMP_OUTPUT"

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

for file in tests/data/*.txt tests/data/*.bin; do
    run_test "$file"
done

echo "=================="
echo "Passed: $passed"
echo "Failed: $failed"

rm -f $TMP_ARCHIVE $TMP_OUTPUT

if [ $failed -ne 0 ]; then
    exit 1
fi

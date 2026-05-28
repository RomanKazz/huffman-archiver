#!/bin/bash

set -e

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"

ARCHIVER="$SCRIPT_DIR/../../build/huff"
DATA_DIR="$SCRIPT_DIR/../data"
RESULTS="$SCRIPT_DIR/result.csv"

file_size() {
    if stat -f%z "$1" >/dev/null 2>&1; then
        stat -f%z "$1"
    else
        stat -c%s "$1"
    fi
}

now_ns() {
    python3 -c 'import time; print(time.time_ns())'
}

echo "file,type,size,compress_time,decompress_time,ratio" > "$RESULTS"

for file in "$DATA_DIR"/*/*
do
    type=$(basename "$(dirname "$file")")
    name=$(basename "$file")
    size=$(file_size "$file")

    for run in {1..10}
    do
        start=$(now_ns)
        "$ARCHIVER" c "$file" tmp.huff
        end=$(now_ns)

        compress_time=$(echo "($end-$start)/1000000000" | bc -l)

        start=$(now_ns)
        "$ARCHIVER" d tmp.huff tmp.out
        end=$(now_ns)

        decompress_time=$(echo "($end-$start)/1000000000" | bc -l)

        compressed_size=$(file_size tmp.huff)

        ratio=$(echo "$compressed_size/$size" | bc -l)

        echo "$name,$type,$size,$compress_time,$decompress_time,$ratio" >> "$RESULTS"

        rm -f tmp.huff tmp.out
    done
done

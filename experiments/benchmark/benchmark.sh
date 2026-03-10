#!/bin/bash

set -e

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"

ARCHIVER="$SCRIPT_DIR/../../build/huff"
DATA_DIR="$SCRIPT_DIR/../data"
RESULTS="$SCRIPT_DIR/result.csv"

echo "file,type,size,compress_time,decompress_time,ratio" > "$RESULTS"

for file in "$DATA_DIR"/*/*
do
    type=$(basename "$(dirname "$file")")
    name=$(basename "$file")
    size=$(stat -f%z "$file")

    for run in {1..10}
    do
        start=$(date +%s%N)
        "$ARCHIVER" c "$file" tmp.huff
        end=$(date +%s%N)

        compress_time=$(echo "($end-$start)/1000000000" | bc -l)

        start=$(date +%s%N)
        "$ARCHIVER" d tmp.huff tmp.out
        end=$(date +%s%N)

        decompress_time=$(echo "($end-$start)/1000000000" | bc -l)

        compressed_size=$(stat -f%z tmp.huff)

        ratio=$(echo "$compressed_size/$size" | bc -l)

        echo "$name,$type,$size,$compress_time,$decompress_time,$ratio" >> "$RESULTS"

        rm -f tmp.huff tmp.out
    done
done

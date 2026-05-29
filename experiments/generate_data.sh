#!/bin/bash

set -e

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
DATA_DIR="$SCRIPT_DIR/data"
BINARY_SOURCE="${BINARY_SOURCE:-/bin/ls}"

if [ ! -f "$BINARY_SOURCE" ]; then
    BINARY_SOURCE="/usr/bin/ls"
fi

echo "Preparing directories..."

rm -rf "$DATA_DIR"
mkdir -p "$DATA_DIR/text"
mkdir -p "$DATA_DIR/random"
mkdir -p "$DATA_DIR/repeat"
mkdir -p "$DATA_DIR/binary"

base="Lorem ipsum dolor sit amet consectetur adipiscing elit."

sizes=("1K:1024" "10K:10240" "100K:102400" "1M:1048576")

echo "Generating text files..."

for entry in "${sizes[@]}"
do
    name=${entry%%:*}
    bytes=${entry##*:}

    yes "$base" | head -c "$bytes" > "$DATA_DIR/text/text_$name.txt"
done

echo "Generating repeat files..."

for entry in "${sizes[@]}"
do
    name=${entry%%:*}
    bytes=${entry##*:}

    yes "a" | head -c "$bytes" > "$DATA_DIR/repeat/repeat_$name.txt"
done

echo "Generating random files..."

for entry in "${sizes[@]}"
do
    name=${entry%%:*}
    bytes=${entry##*:}

    head -c "$bytes" /dev/urandom > "$DATA_DIR/random/random_$name.bin"
done

echo "Generating binary-like files..."

for entry in "${sizes[@]}"
do
    name=${entry%%:*}
    bytes=${entry##*:}
    output="$DATA_DIR/binary/bin_$name.bin"

    : > "$output"
    while [ "$(wc -c < "$output" | tr -d ' ')" -lt "$bytes" ]
    do
        cat "$BINARY_SOURCE" >> "$output"
    done

    tmp="$output.tmp"
    head -c "$bytes" "$output" > "$tmp"
    mv "$tmp" "$output"
done

echo "Done."

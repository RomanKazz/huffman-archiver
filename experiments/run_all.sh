#!/bin/bash

set -e

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
ROOT_DIR="$(cd "$SCRIPT_DIR/.." && pwd)"

echo "Building project..."
cmake -S "$ROOT_DIR" -B "$ROOT_DIR/build"
cmake --build "$ROOT_DIR/build"

echo "Generating data..."
"$SCRIPT_DIR/generate_data.sh"

echo "Running benchmarks..."
"$SCRIPT_DIR/benchmark/benchmark.sh"

echo "Analyzing results..."
python3 "$SCRIPT_DIR/analyze.py"

echo "Done."

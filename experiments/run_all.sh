#!/bin/bash

set -e

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"

echo "Building project..."
make

echo "Generating data..."
"$SCRIPT_DIR/generate_data.sh"

echo "Running benchmarks..."
"$SCRIPT_DIR/benchmark/benchmark.sh"

echo "Analyzing results..."
python3 "$SCRIPT_DIR/analyze.py"

echo "Done."

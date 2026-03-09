CC = gcc
CFLAGS = -Wall -Wextra -O2 -fsanitize=address -Iinclude

BUILD = build
SRC_DIR = src
TEST_DIR = tests/unit
LIB_SRC = $(filter-out $(SRC_DIR)/main.c,$(SRC))

SRC = $(SRC_DIR)/main.c \
      $(SRC_DIR)/huffman_tree.c \
      $(SRC_DIR)/huffman_codec.c \
      $(SRC_DIR)/bitio.c

OBJ = $(SRC:$(SRC_DIR)/%.c=$(BUILD)/%.o)

TARGET = $(BUILD)/huff

TEST_SRC = $(wildcard $(TEST_DIR)/test_*.c)
TEST_BIN = $(TEST_SRC:$(TEST_DIR)/%.c=$(BUILD)/%)

.PHONY: all clean test test_unit bm

all: $(TARGET)

$(TARGET): $(OBJ)
	mkdir -p $(BUILD)
	$(CC) $(CFLAGS) $(OBJ) -o $@

$(BUILD)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

test: $(TARGET)
	./tests/test.sh

bm:
	./tests/benchmark.sh

test_unit: $(TEST_BIN)
	mkdir -p tests/tmp
	for t in $(TEST_BIN); do ./$$t; done

check: test test_unit

$(BUILD)/test_%: tests/unit/test_%.c
	mkdir -p $(BUILD)
	$(CC) $(CFLAGS) -Iinclude $< $(LIB_SRC) -o $@

clean:
	rm -rf $(BUILD)

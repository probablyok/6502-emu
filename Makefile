# Compiler flags
CC := gcc
CFLAGS := -std=c99 -Wall -Wextra -Wpedantic
DEBUG := -g
LDFLAGS :=

# Directories
SRC_DIR := src
INC_DIR := include
OBJ_DIR := obj
TEST_DIR := tests

# Add include directory
CFLAGS += -I$(INC_DIR)

# Target executable and src/obj mappings
TARGET := 6502-emu
TEST_TARGET := run_tests

# Sources
SRCS := $(wildcard $(SRC_DIR)/*.c)
LIB_SRCS := $(filter-out $(SRC_DIR)/main.c, $(SRCS))

# Objects
LIB_OBJS := $(LIB_SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
MAIN_OBJ := $(OBJ_DIR)/main.o

# Test sources + objects
TEST_SRCS := $(wildcard $(TEST_DIR)/*.c)
TEST_OBJS := $(TEST_SRCS:$(TEST_DIR)/%.c=$(OBJ_DIR)/test_%.o)

TEST_JSON_REPO_URL := https://github.com/SingleStepTests/65x02.git
TEST_JSON_DIR := tests/test_json
TEST_JSON_REPO_DIR := 6502/v1

.DEFAULT_GOAL := all
.PHONY: all debug test clean clean-tests fetch-tests

# Build main application
all: $(TARGET)

$(TARGET): $(LIB_OBJS) $(MAIN_OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Get JSON tests from remote repo
fetch-tests:
	@if [ ! -d "$(TEST_JSON_DIR)" ]; then \
		echo "Fetching JSON test cases..."; \
		git clone --depth 1 --filter=blob:none --sparse $(TEST_JSON_REPO_URL) $(TEST_JSON_DIR); \
		git -C $(TEST_JSON_DIR) sparse-checkout set $(TEST_JSON_REPO_DIR); \
	else \
		echo "JSON test cases present. Update with 'git -C $(TEST_JSON_DIR) pull'."; \
	fi

clean-tests:
	rm -rf $(TEST_JSON_DIR)

# Build and run the test suite
test: fetch-tests $(TEST_TARGET)
	./$(TEST_TARGET)

$(TEST_TARGET): $(LIB_OBJS) $(TEST_OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Compile standard C sources
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Compile test C sources
$(OBJ_DIR)/test_%.o: $(TEST_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Create object directory
$(OBJ_DIR):
	mkdir -p $@

# Build variants
debug: CFLAGS += $(DEBUG)
debug: clean $(TARGET)

# Clean artifacts
clean:
	rm -rf $(TARGET)
	rm -rf $(TEST_TARGET)
	rm -rf $(OBJ_DIR)

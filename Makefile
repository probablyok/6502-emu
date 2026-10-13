# Compiler flags
CC := gcc
CFLAGS := -std=c99 -Wall -Wextra -Wpedantic
DEBUG := -g
LDFLAGS :=

# Directories
SRC_DIR := src
INC_DIR := include
OBJ_DIR := obj

# Add include directory
CFLAGS += -I$(INC_DIR)

# Target executable and src/obj mappings
TARGET := 6502-emu
SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

.DEFAULT_GOAL := all
.PHONY: all debug clean

# Build all targets
all: $(TARGET)

# Link executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LDFLAGS)

# Compile C sources
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Create object directory if it does not exist
$(OBJ_DIR):
	mkdir -p $@

# Build variants
debug: CFLAGS += $(DEBUG)
debug: clean $(TARGET)

# Clean artifacts
clean:
	rm -rf $(OBJ_DIR) $(TARGET)
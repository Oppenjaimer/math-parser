SRC_DIR = src
INC_DIR = include
BUILD_DIR = build
BIN_DIR = bin
LIB_DIR = lib

EXEC_TARGET = $(BIN_DIR)/main
LIB_TARGET = $(LIB_DIR)/libmathparser.a

CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -pedantic -I$(INC_DIR)
LDLIBS = -lm

SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
MAIN_SRC = $(SRC_DIR)/main.c
LIB_SRCS = $(filter-out $(MAIN_SRC), $(SRC_FILES))

MAIN_OBJ = $(MAIN_SRC:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)
LIB_OBJS = $(LIB_SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

all: $(LIB_TARGET) $(EXEC_TARGET)

$(EXEC_TARGET): $(MAIN_OBJ) $(LIB_TARGET) | $(BIN_DIR)
	$(CC) $(MAIN_OBJ) -L$(LIB_DIR) -lmathparser $(LDLIBS) -o $@

$(LIB_TARGET): $(LIB_OBJS) | $(LIB_DIR)
	ar rcs $@ $^

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

$(BIN_DIR):
	@mkdir -p $(BIN_DIR)

$(LIB_DIR):
	@mkdir -p $(LIB_DIR)

clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR) $(LIB_DIR)

.PHONY: all clean
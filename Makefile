CC = gcc
CLI_LIB_DIR ?= ../cli-lib
CFLAGS = -Wall -Werror -g -Isrc -I$(CLI_LIB_DIR)/include

TARGET = build/cosmo-dash
BUILD_DIR = build
OBJ_DIR = build/obj

CLI_LIB_SRC = $(CLI_LIB_DIR)/src

PROJ_SRCS = $(wildcard src/*.c)
PROJ_OBJS = $(patsubst src/%.c,$(OBJ_DIR)/proj/%.o,$(PROJ_SRCS))

CLI_SRCS = $(CLI_LIB_SRC)/keyboard.c $(CLI_LIB_SRC)/screen.c $(CLI_LIB_SRC)/timer.c
CLI_OBJS = $(OBJ_DIR)/cli/keyboard.o $(OBJ_DIR)/cli/screen.o $(OBJ_DIR)/cli/timer.o

OBJS = $(PROJ_OBJS) $(CLI_OBJS)

.PHONY: all run clean

all: $(TARGET)

$(TARGET): $(OBJS)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

$(OBJ_DIR)/proj/%.o: src/%.c
	@mkdir -p $(OBJ_DIR)/proj
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/cli/keyboard.o: $(CLI_LIB_SRC)/keyboard.c
	@mkdir -p $(OBJ_DIR)/cli
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/cli/screen.o: $(CLI_LIB_SRC)/screen.c
	@mkdir -p $(OBJ_DIR)/cli
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/cli/timer.o: $(CLI_LIB_SRC)/timer.c
	@mkdir -p $(OBJ_DIR)/cli
	$(CC) $(CFLAGS) -c $< -o $@

run: all
	./build/cosmo-dash

clean:
	rm -rf $(BUILD_DIR)
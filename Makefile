LIBNAME = tge
BUILD_DIR = build
TARGET_LIB = $(BUILD_DIR)/lib$(LIBNAME).so

SRC = $(shell find . -type f -name "*.c" ! -name "main.c" ! -name "test.c")

OBJS = $(SRC:%.c=$(BUILD_DIR)/%.o)

CC = gcc
CFLAGS = -Wall -O2 -march=native -fPIC
LDFLAGS = -shared

.PHONY: all clean

all: $(TARGET_LIB)

$(TARGET_LIB): $(OBJS)
	@$(CC) $(LDFLAGS) -o $@ $^

$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)
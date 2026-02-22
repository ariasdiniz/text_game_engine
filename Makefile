LIBNAME = tge
BUILD_DIR = build
TARGET_LIB = $(BUILD_DIR)/lib$(LIBNAME).so
TARGET_DEMO = damp_cave

SRC = $(shell find . -type f -name "*.c" ! -name "main.c" ! -name "test.c" ! -wholename "*/demo/*")

OBJS = $(SRC:%.c=$(BUILD_DIR)/%.o)

DEMO_SRC = $(shell find demo -type f)

CC = gcc
CFLAGS = -Wall -g -march=native -fPIC
LDFLAGS = -shared
DEMO_FLAGS = -L$(BUILD_DIR) -l$(LIBNAME) -Wl,-rpath,$(BUILD_DIR) -g

.PHONY: all clean

all: $(TARGET_LIB) $(TARGET_DEMO)

$(TARGET_DEMO): $(DEMO_SRC)
	gcc tge.h $^ $(DEMO_FLAGS) -o $(TARGET_DEMO)

$(TARGET_LIB): $(OBJS)
	@$(CC) $(LDFLAGS) -o $@ $^

$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR) $(TARGET_DEMO)
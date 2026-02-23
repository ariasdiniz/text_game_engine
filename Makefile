LIBNAME = tge
BUILD_DIR = build
DEMO_NAME = damp_cave

SRC = $(shell find . -maxdepth 2 -type f -name "*.c" ! -name "main.c" ! -name "test.c" ! -path "./demo/*")
DEMO_SRC = $(shell find demo -type f -name "*.c")

PLATFORM ?= linux
ARCH ?= x86_64

ifeq ($(PLATFORM),windows)
    CC = x86_64-w64-mingw32-gcc
    EXT_LIB = dll
    EXT_BIN = .exe
    CFLAGS_PLAT = -D_WIN32
    LDFLAGS_LIB = -shared -Wl,--out-implib,$(BUILD_DIR)/lib$(LIBNAME).a
else ifeq ($(PLATFORM),mac)
    CC = clang
    EXT_LIB = dylib
    EXT_BIN = 
    CFLAGS_PLAT = -dynamiclib
    LDFLAGS_LIB = -dynamiclib -install_name @rpath/lib$(LIBNAME).dylib
else
    CC = gcc
    EXT_LIB = so
    EXT_BIN = 
    CFLAGS_PLAT = -fPIC
    LDFLAGS_LIB = -shared
endif

ifeq ($(ARCH),arm64)
    ifeq ($(PLATFORM),linux)
        CC = aarch64-linux-gnu-gcc
    else ifeq ($(PLATFORM),mac)
        CFLAGS_PLAT += -arch arm64
    endif
else
    ifeq ($(PLATFORM),mac)
        CFLAGS_PLAT += -arch x86_64
    endif
endif

OUT_DIR = $(BUILD_DIR)/$(PLATFORM)_$(ARCH)
TARGET_LIB = $(OUT_DIR)/lib$(LIBNAME).$(EXT_LIB)
TARGET_DEMO = $(OUT_DIR)/$(DEMO_NAME)$(EXT_BIN)

CFLAGS = -Wall -Wno-unused-result -O2 $(CFLAGS_PLAT) -I.
OBJS = $(SRC:%.c=$(OUT_DIR)/%.o)

.PHONY: all clean

all: $(TARGET_LIB) $(TARGET_DEMO)

$(TARGET_LIB): $(OBJS)
	@mkdir -p $(dir $@)
	$(CC) $(LDFLAGS_LIB) -o $@ $^

$(TARGET_DEMO): $(DEMO_SRC)
	$(CC) $(CFLAGS) $^ -L$(OUT_DIR) -l$(LIBNAME)  -Wl,-rpath,$(OUT_DIR) -o $@

$(OUT_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)
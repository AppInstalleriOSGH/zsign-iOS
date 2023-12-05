CC := clang
CFLAGS := -Wall -Werror $(shell pkg-config --cflags libcrypto) -fPIC -Wno-pointer-to-int-cast -Wno-unused-command-line-argument -Wno-deprecated-declarations -framework CoreFoundation
DEBUG ?= 0

LIB_NAME := libchoma

BUILD_DIR := build/ios
OUTPUT_DIR := output/ios
CFLAGS += -arch arm64 -isysroot $(shell xcrun --sdk iphoneos --show-sdk-path) -miphoneos-version-min=14.0 external/ios/libcrypto.a

HEADER_OUTPUT_DIR := $(OUTPUT_DIR)/include

LIB_DIR := $(OUTPUT_DIR)/lib

STATIC_LIB := $(LIB_DIR)/$(LIB_NAME).a
DYNAMIC_LIB := $(LIB_DIR)/$(LIB_NAME).dylib

SRC_FILES := $(wildcard *.cpp common/*.cpp)
OBJ_FILES := $(patsubst %.cpp,$(BUILD_DIR)/%.o,$(SRC_FILES))

CHOMA_HEADERS_SRC_DIR := $(shell pwd)
CHOMA_HEADERS_DST_DIR := $(HEADER_OUTPUT_DIR)/choma

CHOMA_HEADERS := $(shell find $(CHOMA_HEADERS_SRC_DIR) -type f -name "*.h")

all: $(STATIC_LIB) $(DYNAMIC_LIB) copy-choma-headers

$(STATIC_LIB): $(OBJ_FILES)
	@mkdir -p $(LIB_DIR)
	ar rcs $@ $^

$(DYNAMIC_LIB): $(OBJ_FILES)
	@mkdir -p $(LIB_DIR)
	$(CC) $(CFLAGS) -shared -o $@ $^

$(BUILD_DIR)/%.o: $%.cpp
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

copy-choma-headers: $(CHOMA_HEADERS)
	@rm -rf $(CHOMA_HEADERS_DST_DIR)
	@mkdir -p $(CHOMA_HEADERS_DST_DIR)
	@cp $^ $(CHOMA_HEADERS_DST_DIR)

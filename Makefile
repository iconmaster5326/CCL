# vars
ifeq ($(OS),Windows_NT)
EXE_SUFFIX := .exe
LIB_PREFIX := lib
LIB_SUFFIX := .so
AR_PREFIX := lib
AR_SUFFIX := .a
else
EXE_SUFFIX :=
LIB_PREFIX := lib
LIB_SUFFIX := .so
AR_PREFIX := lib
AR_SUFFIX := .a
endif

BUILD_DIR := $(abspath build)
BIN_DIR := $(abspath bin)
INCLUDE_DIR := $(abspath include)

CXXFLAGS := -std=c++17 -I$(INCLUDE_DIR) -g
CXXFLAGS_SHARED := -fPIC
LINKFLAGS := -rdynamic -lpthread
LINKFLAGS_SHARED := -shared

# main tasks
include src/Makefile
include test/Makefile

# directories
$(BUILD_DIR):
	mkdir $(BUILD_DIR)

$(BIN_DIR):
	mkdir $(BIN_DIR)

# phony rules
all: $(EXE_FILES)

clean:
	rm -rf $(BUILD_DIR)

spotless: clean
	rm -rf $(BIN_DIR)
	
check: $(TEST_EXE_FILE)
	$(TEST_EXE_FILE) --log_level=test_suite

.PHONY: clean spotless all check

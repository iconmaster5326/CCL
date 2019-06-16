EXE_NAME := $(BIN_DIR)/ccl
EXE_FILE := $(EXE_NAME)$(EXE_SUFFIX)
EXE_FILES += $(EXE_FILE)

CXX_FILE := $(wildcard $(CWD)/main/*.cpp)
O_FILE := $(BUILD_DIR)/main.o
D_FILE := $(BUILD_DIR)/main.d

$(EXE_FILE): $(O_FILES) $(O_FILE) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $(EXE_NAME) $(O_FILES) $(O_FILE) $(LINKFLAGS)

$(D_FILE): | $(BUILD_DIR)
$(D_FILE): $(CXX_FILE)
	$(CXX) -MM -MT '$(O_FILE) $(D_FILE)' $(CXXFLAGS) $(CXX_FILE) > $(D_FILE)
include $(D_FILE)

$(O_FILE): | $(BUILD_DIR)
$(O_FILE): $(D_FILE)
	$(CXX) $(CXXFLAGS) -c -o $(O_FILE) $(CXX_FILE)

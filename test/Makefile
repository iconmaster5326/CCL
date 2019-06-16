TEST_CWD := $(abspath test)

TEST_EXE_NAME := $(BIN_DIR)/ccl-tests
TEST_EXE_FILE := $(TEST_EXE_NAME)$(EXE_SUFFIX)
TEST_EXE_FILES += $(TEST_EXE_FILE)

TEST_CXX_FILES := $(wildcard $(TEST_CWD)/*.cpp)
TEST_O_FILES := $(patsubst $(TEST_CWD)/%.cpp,$(BUILD_DIR)/%.o,$(TEST_CXX_FILES))
TEST_D_FILES := $(patsubst $(TEST_CWD)/%.cpp,$(BUILD_DIR)/%.d,$(TEST_CXX_FILES))

TEST_LINKFLAGS := -lboost_unit_test_framework

# executables
$(TEST_EXE_FILE): $(TEST_O_FILES) $(O_FILES) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $(TEST_EXE_NAME) $(TEST_O_FILES) $(O_FILES) $(LINKFLAGS) $(TEST_LINKFLAGS)

# source files
$(TEST_D_FILES): | $(BUILD_DIR)
$(TEST_D_FILES): $(BUILD_DIR)/%.d: $(TEST_CWD)/%.cpp
	$(CXX) -MM -MT '$(patsubst %.d,%.o,$@) $@' $(CXXFLAGS) $< > $@
include $(TEST_D_FILES)

$(TEST_O_FILES): | $(BUILD_DIR)
$(TEST_O_FILES): $(BUILD_DIR)/%.o: $(BUILD_DIR)/%.d
	$(CXX) $(CXXFLAGS) -c -o $@ $(patsubst $(BUILD_DIR)/%.d,$(TEST_CWD)/%.cpp,$<)

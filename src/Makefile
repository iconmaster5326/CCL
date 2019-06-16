CWD := $(abspath src)

LIB_FILE := $(BIN_DIR)/$(LIB_PREFIX)ccl$(LIB_SUFFIX)
AR_FILE := $(BIN_DIR)/$(AR_PREFIX)ccl$(AR_SUFFIX)

CXX_FILES := $(wildcard $(CWD)/*.cpp)
O_FILES := $(patsubst $(CWD)/%.cpp,$(BUILD_DIR)/%.o,$(CXX_FILES))
LO_FILES := $(patsubst $(CWD)/%.cpp,$(BUILD_DIR)/%.lo,$(CXX_FILES))
D_FILES := $(patsubst $(CWD)/%.cpp,$(BUILD_DIR)/%.d,$(CXX_FILES))
EXE_FILES += $(LIB_FILE) $(AR_FILE)

# executables
# (main exe must be at top, so it's default)
include src/main/Makefile

$(LIB_FILE): $(LO_FILES) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $(CXXFLAGS_SHARED) -o $(LIB_FILE) $(LO_FILES) $(LINKFLAGS) $(LINKFLAGS_SHARED)

$(AR_FILE): $(O_FILES) | $(BIN_DIR)
	$(AR) r $(AR_FILE) $(O_FILES)

# source files
$(D_FILES): | $(BUILD_DIR)
$(D_FILES): $(BUILD_DIR)/%.d: $(CWD)/%.cpp
	$(CXX) -MM -MT '$(patsubst %.d,%.o,$@) $@' $(CXXFLAGS) $< > $@
include $(D_FILES)

$(O_FILES): | $(BUILD_DIR)
$(O_FILES): $(BUILD_DIR)/%.o: $(BUILD_DIR)/%.d
	$(CXX) $(CXXFLAGS) -c -o $@ $(patsubst $(BUILD_DIR)/%.d,$(CWD)/%.cpp,$<)

$(LO_FILES): | $(BUILD_DIR)
$(LO_FILES): $(BUILD_DIR)/%.lo: $(BUILD_DIR)/%.d
	$(CXX) $(CXXFLAGS) $(CXXFLAGS_SHARED) -c -o $@ $(patsubst $(BUILD_DIR)/%.d,$(CWD)/%.cpp,$<)

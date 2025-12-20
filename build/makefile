# ===== Toolchain =====
CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -Werror -O0 -g 
ROOT := ..
INCLUDES := -I$(ROOT)/include
# ===== Paths =====
BUILD_DIR := build
HW_SRC := $(ROOT)/hw_model/fake_hw.cpp
HW_OBJ := $(BUILD_DIR)/fake_hw.o
FW_SRC := $(ROOT)/firmware/main.cpp
FW_OBJ := $(BUILD_DIR)/firmware_main.o
BROKEN_FW_SRC := $(ROOT)/firmware/broken_fw.cpp
BROKEN_FW_OBJ := $(BUILD_DIR)/broken_fw.o
HW_BIN := $(BUILD_DIR)/fake_hw

# ===== Default target =====
all: $(HW_BIN)

# ===== Build binary =====
$(HW_BIN): $(HW_OBJ) $(FW_OBJ) $(BROKEN_FW_OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@

# ===== Compile object =====
$(HW_OBJ): $(HW_SRC) $(ROOT)/include/registers.hpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(FW_OBJ): $(FW_SRC) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(BROKEN_FW_OBJ): $(BROKEN_FW_SRC) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@
	
# ===== Create build dir =====
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# ===== Clean =====
clean:
	rm -rf $(BUILD_DIR)
	
run: $(HW_BIN)
	./$(HW_BIN)

.PHONY: all clean

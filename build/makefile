# ===== Toolchain =====
CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -Werror -O0 -g 
ROOT := ..
INCLUDES := -I$(ROOT)/include
# ===== Paths =====
BUILD_DIR := build
HW_SRC := $(ROOT)/hw_model/fake_hw.cpp
HW_OBJ := $(BUILD_DIR)/fake_hw.o
HW_NO_QUEUE_SRC := $(ROOT)/hw_model/fake_hw_no_queue.cpp
HW_NO_QUEUE_OBJ := $(BUILD_DIR)/fake_hw_no_queue.o

FW_SRC := $(ROOT)/firmware/main.cpp
FW_OBJ := $(BUILD_DIR)/firmware_main.o

FW_SINGLE_THREAD_SRC := $(ROOT)/firmware/fw_one_thread_queue.cpp
FW_SINGLE_THREAD_OBJ := $(BUILD_DIR)/fw_one_thread_queue.o


FW_SINGLE_THREAD_NO_QUEUE_SRC := $(ROOT)/firmware/fw_one_thread_no_queue.cpp
FW_SINGLE_THREAD_NO_QUEUE_OBJ := $(BUILD_DIR)/fw_one_thread_no_queue.o


HW_BIN := $(BUILD_DIR)/fake_hw

# ===== Default target =====
all: $(HW_BIN)

# ===== Build binary =====
$(HW_BIN): $(HW_OBJ) $(HW_NO_QUEUE_OBJ) $(FW_OBJ) $(FW_SINGLE_THREAD_NO_QUEUE_OBJ) $(FW_SINGLE_THREAD_OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@

# ===== Compile object =====
$(HW_OBJ): $(HW_SRC) $(ROOT)/include/registers.hpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(HW_NO_QUEUE_OBJ): $(HW_NO_QUEUE_SRC) $(ROOT)/include/registers.hpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(FW_OBJ): $(FW_SRC) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(FW_SINGLE_THREAD_OBJ): $(FW_SINGLE_THREAD_SRC) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(FW_SINGLE_THREAD_NO_QUEUE_OBJ): $(FW_SINGLE_THREAD_NO_QUEUE_SRC) | $(BUILD_DIR)
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

#pragma once
#include "registers.hpp"
#include <thread>
#include <atomic>
#include <chrono>
#include <iostream>

namespace gpu::hw {

enum class HwState {
    IDLE, 
    BUSY,
    ERROR
};

void fake_hw_loop(std::atomic<bool>& running);
    uintptr_t get_mmio_base();
} // namespace gpu::hw 
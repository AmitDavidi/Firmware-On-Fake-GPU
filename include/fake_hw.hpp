#pragma once
#include "registers.hpp"
#include <thread>
#include <atomic>
#include <chrono>
#include <iostream>

namespace gpu::hw {
    void fake_hw_loop(std::atomic<bool>& running);
} // namespace gpu::hw 
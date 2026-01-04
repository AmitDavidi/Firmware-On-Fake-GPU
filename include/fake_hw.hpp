#pragma once
#include "registers.hpp"
#include <thread>
#include <atomic>
#include <chrono>
#include <iostream>

namespace gpu::hw {

void fake_hw_loop(std::atomic<bool>& running);
uintptr_t get_mmio_base();
#include "registers.hpp"
#include <iostream>
#include <atomic>
#include <thread>
#include <chrono>
#include <vector>
#include <mutex>

using namespace gpu::regs;

namespace gpu::hw {

// Internal FIFO
constexpr uint32_t QUEUE_SIZE = 16;
struct CommandQueue {
    uint32_t data[QUEUE_SIZE]{};
    uint32_t head = 0;
    uint32_t tail = 0;
    uint32_t count = 0;
    std::mutex mtx;

    bool push(uint32_t cmd_id) {
        std::lock_guard<std::mutex> lock(mtx);
        if (count == QUEUE_SIZE) return false;
        data[tail] = cmd_id;
        tail = (tail + 1) % QUEUE_SIZE;
        count++;
        return true;
    }

    bool pop(uint32_t &cmd_id) {
        std::lock_guard<std::mutex> lock(mtx);
        if (count == 0) return false;
        cmd_id = data[head];
        head = (head + 1) % QUEUE_SIZE;
        count--;
        return true;
    }

    bool empty() {
        std::lock_guard<std::mutex> lock(mtx);
        return count == 0;
    }

    bool full() {
        std::lock_guard<std::mutex> lock(mtx);
        return count == QUEUE_SIZE;
    }
};

static CommandQueue cmd_queue;

// MMIO registers exposed to FW
static RegisterBlock hw_regs {};

uintptr_t get_mmio_base() {
    return reinterpret_cast<uintptr_t>(&hw_regs);
}

// ==============================
// RX Handler Thread
// ==============================
void hw_rx_thread(std::atomic<bool>& running);
void hw_executor_thread(std::atomic<bool>& running);

} // namespace gpu::hw 
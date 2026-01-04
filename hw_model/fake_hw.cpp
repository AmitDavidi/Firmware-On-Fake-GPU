#include "registers.hpp"
#include <iostream>
#include <atomic>
#include <thread>
#include <chrono>
#include <cstdlib>

using namespace gpu::regs;

namespace gpu::hw {

// Internal FIFO inside HW
constexpr uint32_t QUEUE_SIZE = 16;
struct CommandQueue {
    uint32_t data[QUEUE_SIZE]{};
    uint32_t head = 0;
    uint32_t tail = 0;
    uint32_t count = 0;

    bool push(uint32_t cmd_id) {
        if (count == QUEUE_SIZE) return false; // full
        data[tail] = cmd_id;
        tail = (tail + 1) % QUEUE_SIZE;
        count++;
        return true;
    }

    bool pop(uint32_t &cmd_id) {
        if (count == 0) return false;
        cmd_id = data[head];
        head = (head + 1) % QUEUE_SIZE;
        count--;
        return true;
    }

    bool empty() const { return count == 0; }
    bool full() const { return count == QUEUE_SIZE; }
};

static CommandQueue cmd_queue;

// MMIO registers exposed to FW
static RegisterBlock hw_regs {};

uintptr_t get_mmio_base() {
    return reinterpret_cast<uintptr_t>(&hw_regs);
}

// ==========================================
// Main fake HW loop
// ==========================================
void fake_hw_loop(std::atomic<bool> &running) {
    using namespace std::chrono_literals;

    while (running.load(std::memory_order_relaxed)) {
        
        //std::this_thread::sleep_for(std::chrono::nanoseconds(1));

        // -------------------------
        // FW submitted a command
        // -------------------------
        if (hw_regs.doorbell) {
            hw_regs.doorbell = 0; // clear doorbell
            
            uint32_t cmd_id = hw_regs.cmd_id; // single command ID
            if (cmd_queue.push(cmd_id)) {
                // std::cout << "[HW] pushed cmd " << cmd_id << " into FIFO\n";
            } else {
                // std::cout << "[HW] FIFO full, drop cmd " << cmd_id << "\n";
            }
            hw_regs.queue_full = cmd_queue.full();
            hw_regs.push_done = 1;
        }

        // -------------------------
        // Process next command in FIFO
        // -------------------------
        if (!cmd_queue.empty()) {
            hw_regs.busy = 1;

            uint32_t cmd;
            if (cmd_queue.pop(cmd)) {
                std::cout << "[HW] executing cmd " << cmd << "\n";

                // Simulate execution time
                std::this_thread::sleep_for(5ms);

                // Optionally inject error
                // if (rand() % 10 == 0) {
                //     hw_regs.irq_status = 1; // error flag
                //     std::cout << "[HW] cmd " << cmd << " caused ERROR\n";
                // }

                hw_regs.done = 1; // FW clears this
                hw_regs.busy = 0;
            }
        }

        // -------------------------
        // FW clears done flag
        // -------------------------
        if (hw_regs.clear_done) {
            hw_regs.done = 0;
            hw_regs.clear_done = 0;
        }

        // tiny sleep to prevent busy spinning
        std::this_thread::sleep_for(100ns);
    }
}

} // namespace gpu::hw

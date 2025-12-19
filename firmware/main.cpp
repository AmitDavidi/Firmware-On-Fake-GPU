#include <thread>
#include <atomic>
#include "fake_hw.hpp"
#include "registers.hpp"
#include <iostream>

using namespace gpu::hw;

int main() {
    std::atomic<bool> running{true};
    
    std::cout << sizeof(gpu::regs::RegisterBlock) << std::endl;

    std::thread hw_thread(fake_hw_loop, std::ref(running));

    FW simulation loop
    run_fw_simulation(hw_regs);

    hw_thread.join();
    return 0;
}


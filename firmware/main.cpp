#include <iostream>
#include <thread>
#include <atomic>
#include <fw_defines.hpp>
#include "fake_hw.hpp"
#include "registers.hpp"
#include "broken_fw.hpp"

using namespace gpu::hw;
using namespace gpu::fw;

int main() {
    std::atomic<bool> running{true};
    
    std::cout << sizeof(gpu::regs::RegisterBlock) << std::endl;

    std::thread hw_thread(fake_hw_loop, std::ref(running));

    fw_broken_coherency();

    hw_thread.join();
    return 0;
}


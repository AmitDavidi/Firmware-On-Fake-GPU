#include <iostream>
#include <thread>
#include <atomic>
#include <fw_defines.hpp>
#include "fake_hw.hpp"
#include "fake_hw_no_queue.hpp"
#include "registers.hpp"
#include "fw_one_thread_no_queue.hpp"
#include "fw_one_thread_queue.hpp"

using namespace gpu::hw;
using namespace gpu::fw;

int main() {
    std::atomic<bool> running{true};
    
    std::cout << sizeof(gpu::regs::RegisterBlock) << std::endl;

    // std::thread hw_thread(fake_hw_loop, std::ref(running));
    // std::thread hw_thread_no_queue(fake_hw_loop_no_queue, std::ref(running));
    // std::thread hw_thread(start_fake_hw, std::ref(running));
    rx_thread = std::thread(hw_rx_thread, std::ref(running));
    executor_thread = std::thread(hw_executor_thread, std::ref(running));

    fw_queue();

    hw_thread.join();
    return 0;
}


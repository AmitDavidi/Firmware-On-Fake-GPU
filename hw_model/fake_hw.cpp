#include "fake_hw.hpp"
#include <iostream>
// Main Hardware thread
// this will simulate the basic hardware operations

// WHY USING NAMESPACE
using namespace gpu::regs;

// WHY STATIC
static RegisterBlock hw_regs {};

namespace gpu::hw {

void fake_hw_loop(std::atomic<bool>& running) {
    // WHY ATOMIC - WHY .LOAD    
    while(running.load()) {
        
        std::cout << "Clock" << std::endl;

        if (hw_regs.control & 0x1) {
            hw_regs.status = 1; // SET BUSY
            std::this_thread::sleep_for(std::chrono::milliseconds((50))); // TODO ADD RANDOMIZATION
            // completed

            hw_regs.cmd_head = hw_regs.cmd_tail;

            // SET IDLE
            hw_regs.status = 0;
            
            // randomize irq 
            hw_regs.irq_status = 0;
        }

    }
}

} // namespace gpu::hw


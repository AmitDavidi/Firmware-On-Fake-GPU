#include "fake_hw.hpp"
#include <iostream>
// ----------------------------------------------------------
// ---              Main Hardware thread                  ---
// ---  this will simulate the basic hardware operations  ---
// ----------------------------------------------------------



using namespace gpu::regs;

// Statis assures:
// this isnt stored in stack or heap.
// Stored in block starting symbol .bss \ Data segment .data
// It has a fixed address determined at link time
// It never moves or reallocates during execution
static RegisterBlock hw_regs {};

// NOTES: 
// I'm an ascii artist
// | ------- |
// |   STACK |
// | ------- |
// |    .    |
// |    .    |
// |    .    |
// |    .    |
// |-------- | 
// |   heap  |
// | ------- |
// |     bss |
// | --------|
// |   data  |
// | --------|
// |   text  |
// | --------|


// This is storring it in stack:
// void fake_hw_loop() {
    // RegisterBlock hw_regs {}; // אסון
// }

// This is storing it in heap:
// auto* hw_regs = new RegisterBlock{};


namespace gpu::hw {


// Direct access breaks the HW/FW contract - FW doesn't have acess to the entire register space
// FW should only see an MMIO interface.
uintptr_t get_mmio_base() {
    return reinterpret_cast<uintptr_t>(&hw_regs);
}


void fake_hw_loop(std::atomic<bool>& running) {
    // WHY ATOMIC - WHY .LOAD - TODO study this library

    while(running.load()) {
        std::this_thread::sleep_for(std::chrono::nanoseconds(1));

        static uint32_t clock_count = 0;
        clock_count++;
        // std::cout << "Clock " << clock_count << std::endl;

        if (hw_regs.control & 0x1) {
            hw_regs.control &= ~0x1;

            hw_regs.status = 1; // SET BUSY
            std::this_thread::sleep_for(std::chrono::milliseconds((50))); // TODO ADD RANDOMIZATION
            // completed

            hw_regs.cmd_head = hw_regs.cmd_tail;

            // SET IDLE
            hw_regs.status = 0;
            hw_regs.control = 0;
            // randomize irq 
            hw_regs.irq_status = 0;

            std::cout << "Clock " << clock_count << " " << hw_regs.cmd_tail << " Completed" << std::endl;
        }

    }
}

} // namespace gpu::hw


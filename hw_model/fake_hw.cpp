#include "fake_hw.hpp"
#include <iostream>
#include <random>

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
using namespace  gpu::hw;



// Direct access breaks the HW/FW contract - FW doesn't have acess to the entire register space
// FW should only see an MMIO interface.
uintptr_t get_mmio_base() {
    return reinterpret_cast<uintptr_t>(&hw_regs);
}
bool generate_irq_error() {
   if (rand() % 10 == 0) {
        return true;
    }
    return false;
}

void fake_hw_loop(std::atomic<bool>& running)
{
    uint32_t latched_cmd = 0;

    while (running.load()) {
        std::this_thread::sleep_for(std::chrono::nanoseconds(1));

        // START edge
        if ((hw_regs.start & 0x1) && hw_regs.busy == 0) {

            latched_cmd = hw_regs.cmd_tail;

            hw_regs.start = 0;      // consume pulse

            hw_regs.busy  = 1;

            hw_regs.done  = 0;
            
            // simulate execution
            std::this_thread::sleep_for(std::chrono::milliseconds(10 + rand()%50));

            // complete
            hw_regs.cmd_head = latched_cmd;
            hw_regs.done = 1;
            hw_regs.busy = 0;

            std::cout
                << "Cmd: " << latched_cmd
                << " Completed\n";
        }

        // FW clears DONE
        if (hw_regs.clear_done) {
            hw_regs.done = 0;
            hw_regs.clear_done = 0;
        }
    }
}




            
} // namespace gpu::hw


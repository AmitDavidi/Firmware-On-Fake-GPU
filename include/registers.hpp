#pragma once
#include <cstdint>

namespace gpu::regs {

struct alignas(4) RegisterBlock {
    // volatile because we model hardware here - we don't want the compiler to optimize access to those registers
    // every access must happen. we must assume these registers change outside the main program flow.
    volatile uint32_t busy; 
    volatile uint32_t cmd_head;
    volatile uint32_t cmd_tail;
    volatile uint32_t irq_status;
    volatile uint32_t start; 
    volatile uint32_t done;
    volatile uint32_t clear_done;
};

struct BadRegs {
    uint32_t status;   // 0x00
    uint8_t cmd_head;  // 0x04 ??? actually padding added
    uint32_t cmd_tail; // ??? bad offset
    // in this case - fw will write to byte 0x5 to access cmd_tail. but hw will not understand since cmd_tail correct offset is 0x8. - BUG
};


// the total byte size of RegisterBlock must be 4Bytes * 5 Registers = 20Bytes
static_assert(sizeof(RegisterBlock) == 0x1C);

} // namespace gpu::regs
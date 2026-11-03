#pragma once
#include "cpu_state.hpp"
#include "instruction_decoder.hpp"

namespace M68K {
    class CPU {
    private:
        bool take_interrupt();

    public:
        CPU() = default;

        CPUState state = CPUState();
        InstructionDecoder instruction_decoder = InstructionDecoder();

        void step();
        void reset();
        void interrupt(uint8_t level);
        bool load_elf(const std::string& filename);
    };
}

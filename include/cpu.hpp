#pragma once
#include "cpu_state.hpp"
#include "instruction_decoder.hpp"

namespace M68K{
    class CPU{
    private:
    
    public:
        CPU() = default;
        CPU(CPU&) = default;

        CPUState state = CPUState();
        InstructionDecoder instruction_decoder = InstructionDecoder();

        void step();
        bool loadELF(const std::string& filename);
    };
}
#pragma once

#include "defines.hpp"
#include "cpu_state.hpp"

#include <cstdint>

namespace M68K{
    namespace INSTRUCTION{
        class Instruction{
        protected:
            uint16_t opcode;
            bool is_valid = true;
            size_t instruction_size = 2;
        public:
            Instruction(uint16_t opcode) : opcode(opcode) {};
            virtual void execute(CPUState&) {};
        };
    }
}

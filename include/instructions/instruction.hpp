#pragma once

#include "defines.hpp"
#include "cpu_state.hpp"

#include <cstdint>

namespace M68K{
    namespace INSTRUCTION{
        class Instruction{
        protected:
            uint16_t opcode;
            size_t instruction_size = 2;
        public:
            bool is_valid = true;

            Instruction(uint16_t opcode) : opcode(opcode) {};
            virtual void execute(CPUState&) {};
        };
    }
}

#pragma once

#include "defines.hpp"
#include "cpu_state.hpp"
#include "disassembler.hpp"

#include <string>
#include <cstdint>

namespace M68K{
    namespace INSTRUCTION{
        class Instruction{
        protected:
            uint16_t opcode;

        public:
            bool is_valid = true;

            Instruction(uint16_t opcode) : opcode(opcode) {};
            virtual void execute(CPUState&) {};
            virtual std::string disassembly(CPUState&) { return "unknown"; };

            static AddressingMode getAddressingMode(uint16_t part_mode, uint16_t part_reg);
            static RegisterType getRegisterType(uint16_t part_mode, uint16_t part_reg);
            static Condition getCondition(uint16_t cond_part);
        };
    }
}

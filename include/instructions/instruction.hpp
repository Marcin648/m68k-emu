#pragma once

#include "defines.hpp"
#include "cpu_state.hpp"
#include "disassembler.hpp"

#include <string>
#include <cstdint>

namespace M68K {
    namespace INSTRUCTION {
        class Instruction {
        protected:
            uint16_t opcode;

        public:
            bool is_valid = true;

            Instruction(uint16_t opcode) : opcode(opcode) {};
            virtual ~Instruction() = default;
            virtual void execute(CPUState&) const {};
            virtual std::string disassembly(CPUState&) const { return "unknown"; };

            static AddressingMode get_addressing_mode(uint16_t part_mode, uint16_t part_reg);
            static RegisterType get_register_type(uint16_t part_mode, uint16_t part_reg);
            static Condition get_condition(uint16_t cond_part);
        };
    }
}

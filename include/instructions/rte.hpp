#pragma once
#include "instruction.hpp"
#include <memory>

namespace M68K {
    namespace INSTRUCTION {
        class Rte : public Instruction {
        public:
            Rte(uint16_t opcode) : Instruction(opcode) {};
            void execute(CPUState& cpu_state) const override;
            std::string disassembly(CPUState& cpu_state) const override;

            static std::unique_ptr<INSTRUCTION::Instruction> create(uint16_t opcode);
        };
    }
}

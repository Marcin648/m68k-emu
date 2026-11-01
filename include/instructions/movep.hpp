#pragma once
#include "instruction.hpp"
#include <memory>

namespace M68K {
    namespace INSTRUCTION {
        class Movep : public Instruction {
        private:
            RegisterType data_reg = REG_D0;
            RegisterType addr_reg = REG_A0;
            bool to_memory = false;

            DataSize data_size;

        public:
            Movep(uint16_t opcode);
            void execute(CPUState& cpu_state) const override;
            std::string disassembly(CPUState& cpu_state) const override;

            static std::unique_ptr<INSTRUCTION::Instruction> create(uint16_t opcode);
        };
    }
}

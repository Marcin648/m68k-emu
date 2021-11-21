#pragma once
#include "instruction.hpp"
#include <memory>

namespace M68K{
    namespace INSTRUCTION{
        class Pea : public Instruction{
        private:
            AddressingMode addr_mode = ADDR_MODE_INDIRECT;
            RegisterType addr_reg = REG_D0;

            DataSize data_size = SIZE_LONG;
        public:
            Pea(uint16_t opcode);
            void execute(CPUState& cpu_state) override;

            static std::shared_ptr<INSTRUCTION::Instruction> create(uint16_t opcode);
        };
    }
}

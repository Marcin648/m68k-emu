#pragma once
#include "instruction.hpp"
#include <memory>

namespace M68K{
    namespace INSTRUCTION{
        class Lea : public Instruction{
        private:
            AddressingMode dest_mode = ADDR_MODE_INDIRECT;
            RegisterType dest_reg = REG_D0;
            RegisterType addr_reg = REG_A0;

            DataSize data_size = SIZE_LONG;
        public:
            Lea(uint16_t opcode);
            void execute(CPUState& cpu_state) override;

            static std::shared_ptr<INSTRUCTION::Instruction> create(uint16_t opcode);
        };
    }
}

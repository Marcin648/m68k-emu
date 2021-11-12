#pragma once
#include "instruction.hpp"
#include <memory>

namespace M68K{
    namespace INSTRUCTION{
        class Suba : public Instruction{
        private:
            AddressingMode dest_mode = ADDR_MODE_UNKNOWN;
            AddressingMode src_mode = ADDR_MODE_UNKNOWN;

            RegisterType src_reg = REG_D0;
            RegisterType dest_reg = REG_A0;

            DataSize data_size;
        public:
            Suba(uint16_t opcode);
            void execute(CPUState& cpu_state) override;

            static std::shared_ptr<INSTRUCTION::Instruction> create(uint16_t opcode);
        };
    }
}

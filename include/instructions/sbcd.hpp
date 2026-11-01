#pragma once
#include "instruction.hpp"
#include <memory>

namespace M68K {
    namespace INSTRUCTION {
        class Sbcd : public Instruction {
        private:
            AddressingMode src_mode = ADDR_MODE_DIRECT_DATA;
            RegisterType src_reg = REG_D0;
            AddressingMode dest_mode = ADDR_MODE_DIRECT_DATA;
            RegisterType dest_reg = REG_D0;

        public:
            Sbcd(uint16_t opcode);
            void execute(CPUState& cpu_state) const override;
            std::string disassembly(CPUState& cpu_state) const override;

            static std::unique_ptr<INSTRUCTION::Instruction> create(uint16_t opcode);
        };
    }
}

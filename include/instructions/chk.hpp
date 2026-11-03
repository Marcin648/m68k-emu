#pragma once
#include "instruction.hpp"
#include <memory>

namespace M68K {
    namespace INSTRUCTION {
        class Chk : public Instruction {
        private:
            AddressingMode src_mode = ADDR_MODE_UNKNOWN;
            RegisterType src_reg = REG_D0;
            RegisterType data_reg = REG_D0;

        public:
            Chk(uint16_t opcode);
            void execute(CPUState& cpu_state) const override;
            std::string disassembly(CPUState& cpu_state) const override;

            static std::unique_ptr<INSTRUCTION::Instruction> create(uint16_t opcode);
        };
    }
}

#pragma once
#include "instruction.hpp"
#include <memory>

namespace M68K {
    namespace INSTRUCTION {
        class Dbcc : public Instruction {
        private:
            Condition condition = COND_TRUE;
            RegisterType data_reg = REG_D0;

        public:
            Dbcc(uint16_t opcode);
            void execute(CPUState& cpu_state) const override;
            std::string disassembly(CPUState& cpu_state) const override;

            static std::unique_ptr<INSTRUCTION::Instruction> create(uint16_t opcode);
        };
    }
}

#pragma once
#include "instruction.hpp"
#include <memory>

namespace M68K{
    namespace INSTRUCTION{
        class Link : public Instruction{
        private:
            RegisterType addr_reg = REG_A0;
            DataSize data_size = SIZE_WORD;
        public:
            Link(uint16_t opcode);
            void execute(CPUState& cpu_state) override;

            static std::shared_ptr<INSTRUCTION::Instruction> create(uint16_t opcode);
        };
    }
}

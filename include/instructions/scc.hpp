#pragma once
#include "instruction.hpp"
#include <memory>

namespace M68K{
    namespace INSTRUCTION{
        class Scc : public Instruction{
        private:
            AddressingMode dest_mode = ADDR_MODE_DIRECT_DATA;
            RegisterType dest_reg = REG_D0;
            Condition condition = COND_TRUE;
            DataSize data_size = SIZE_BYTE;
        public:
            Scc(uint16_t opcode);
            void execute(CPUState& cpu_state) override;

            static std::shared_ptr<INSTRUCTION::Instruction> create(uint16_t opcode);
        };
    }
}

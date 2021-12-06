#pragma once
#include "instruction.hpp"
#include <memory>

namespace M68K{
    namespace INSTRUCTION{
        class Bcc : public Instruction{
        private:
            Condition condition = COND_TRUE;
            uint8_t displacement_8 = 0;

            DataSize data_size = SIZE_BYTE;
        public:
            Bcc(uint16_t opcode);
            void execute(CPUState& cpu_state) override;
            std::string disassembly(CPUState& cpu_state) override;

            static std::shared_ptr<INSTRUCTION::Instruction> create(uint16_t opcode);
        };
    }
}

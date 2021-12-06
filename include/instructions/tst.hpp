#pragma once
#include "instruction.hpp"
#include <memory>

namespace M68K{
    namespace INSTRUCTION{
        class Tst : public Instruction{
        private:
            AddressingMode ea_mode = ADDR_MODE_INDIRECT;
            RegisterType ea_reg = REG_D0;

            DataSize data_size = SIZE_LONG;
        public:
            Tst(uint16_t opcode);
            void execute(CPUState& cpu_state) override;
            std::string disassembly(CPUState& cpu_state) override;

            static std::shared_ptr<INSTRUCTION::Instruction> create(uint16_t opcode);
        };
    }
}

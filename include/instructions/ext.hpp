#pragma once
#include "instruction.hpp"
#include <memory>

namespace M68K{
    namespace INSTRUCTION{
        class Ext : public Instruction{
        private:
            AddressingMode addr_mode = ADDR_MODE_DIRECT_DATA;
            RegisterType addr_reg = REG_D0;

            DataSize data_size = SIZE_LONG;
        public:
            Ext(uint16_t opcode);
            void execute(CPUState& cpu_state) override;
            std::string disassembly(CPUState& cpu_state) override;

            static std::shared_ptr<INSTRUCTION::Instruction> create(uint16_t opcode);
        };
    }
}

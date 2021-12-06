#pragma once
#include "instruction.hpp"
#include <memory>

namespace M68K{
    namespace INSTRUCTION{
        class Moveq : public Instruction{
        private:
            AddressingMode dest_mode = ADDR_MODE_DIRECT_DATA;
            RegisterType dest_reg = REG_D0;

            uint8_t imm_data = 0;
            DataSize data_size;
        public:
            Moveq(uint16_t opcode);
            void execute(CPUState& cpu_state) override;
            std::string disassembly(CPUState& cpu_state) override;

            static std::shared_ptr<INSTRUCTION::Instruction> create(uint16_t opcode);
        };
    }
}

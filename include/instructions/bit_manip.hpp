#pragma once
#include "instruction.hpp"
#include <memory>

namespace M68K{
    namespace INSTRUCTION{
        class BitManip : public Instruction{
        private:
            enum InstructionType{
                BTST = 0x00,
                BCHG = 0x01,
                BCLR = 0x02,
                BSET = 0x03
            };

            AddressingMode src_mode = ADDR_MODE_DIRECT_DATA;
            RegisterType src_reg = REG_D0;
            AddressingMode dest_mode = ADDR_MODE_UNKNOWN;
            RegisterType dest_reg = REG_D0;

            InstructionType instruction_type = InstructionType::BCHG;
            std::string instruction_str;

            uint8_t data_bits_modulo;
            DataSize data_size;
        public:
            BitManip(uint16_t opcode);
            void execute(CPUState& cpu_state) override;
            std::string disassembly(CPUState& cpu_state) override;

            static std::shared_ptr<INSTRUCTION::Instruction> create(uint16_t opcode);
        };
    }
}

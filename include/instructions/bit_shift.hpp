#pragma once
#include "instruction.hpp"
#include <memory>

namespace M68K{
    namespace INSTRUCTION{
        class BitShift : public Instruction{
        private:
            enum InstructionType{
                AS = 0x00,
                LS = 0x01,
                ROX = 0x02,
                RO = 0x03
            };
            AddressingMode addr_mode = ADDR_MODE_DIRECT_DATA;
            RegisterType addr_reg = REG_D0;
            RegisterType shift_reg = REG_D0;
            uint8_t imm_shift = 0;

            bool direction_left = false;
            bool is_imm = false;
            bool is_memory = false; 

            InstructionType instruction_type = InstructionType::AS;
            std::string instruction_str;
            
            DataSize data_size;
        public:
            BitShift(uint16_t opcode);
            void execute(CPUState& cpu_state) override;
            std::string disassembly(CPUState& cpu_state) override;

            static std::shared_ptr<INSTRUCTION::Instruction> create(uint16_t opcode);
        };
    }
}

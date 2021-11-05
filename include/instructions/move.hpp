#pragma once
#include "instruction.hpp"
#include <memory>

namespace M68K{
    namespace INSTRUCTION{
        class Move : public Instruction{
        private:
            uint16_t dest_part_mode = 0;
            uint16_t dest_part_reg = 0;
            uint16_t src_part_mode = 0;
            uint16_t src_part_reg = 0;

            AddressingMode mode_dest_addr = ADDR_MODE_UNKNOWN;
            AddressingMode mode_src_addr = ADDR_MODE_UNKNOWN;

            RegisterType src_reg = REG_D0;
            RegisterType dest_reg = REG_D0;

            DataSize data_size;
        public:
            Move(uint16_t opcode);
            void execute(CPUState& cpu_state) override;

            static std::shared_ptr<INSTRUCTION::Instruction> create(uint16_t opcode);
        };
    }
}

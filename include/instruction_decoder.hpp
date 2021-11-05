#pragma once
#include "defines.hpp"
#include "instructions.hpp"

#include <vector>
#include <memory>

namespace M68K{
    class InstructionDecoder{
    private:
        std::vector<std::shared_ptr<INSTRUCTION::Instruction>> opcode_table;

        void generateOpcodeTable();
    public:
        InstructionDecoder();
        INSTRUCTION::Instruction* Decode(uint16_t opcode);

        static AddressingMode getAddressingMode(uint16_t part_mode, uint16_t part_reg);
        static RegisterType getRegister(uint16_t part_mode, uint16_t part_reg);
    };
}

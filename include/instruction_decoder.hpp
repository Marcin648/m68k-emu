#pragma once
#include "defines.hpp"
#include "instructions.hpp"

#include <vector>
#include <memory>

namespace M68K{
    class InstructionDecoder{
    private:
        std::vector<std::unique_ptr<INSTRUCTION::Instruction>> opcode_table;

        void generateOpcodeTable();
    public:
        InstructionDecoder();

        const INSTRUCTION::Instruction* Decode(uint16_t opcode) const{
            return this->opcode_table[opcode].get();
        }
    };
}

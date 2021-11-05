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
        InstructionDecoder(InstructionDecoder&) = default;

        INSTRUCTION::Instruction* Decode(uint16_t opcode);
    };
}

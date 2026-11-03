#pragma once
#include "instruction.hpp"
#include <memory>

namespace M68K {
    namespace INSTRUCTION {
        class CcrLogic : public Instruction {
        private:
            enum InstructionType {
                ORI = 0x00,
                ANDI = 0x01,
                EORI = 0x05
            };

            InstructionType instruction_type = InstructionType::ORI;
            std::string instruction_str;
            bool is_sr = false;

        public:
            CcrLogic(uint16_t opcode);
            void execute(CPUState& cpu_state) const override;
            std::string disassembly(CPUState& cpu_state) const override;

            static std::unique_ptr<INSTRUCTION::Instruction> create(uint16_t opcode);
        };
    }
}

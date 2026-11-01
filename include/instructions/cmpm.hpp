#pragma once
#include "instruction.hpp"
#include <memory>

namespace M68K {
    namespace INSTRUCTION {
        class Cmpm : public Instruction {
        private:
            RegisterType src_reg = REG_A0;
            RegisterType dest_reg = REG_A0;

            DataSize data_size;

        public:
            Cmpm(uint16_t opcode);
            void execute(CPUState& cpu_state) const override;
            std::string disassembly(CPUState& cpu_state) const override;

            static std::unique_ptr<INSTRUCTION::Instruction> create(uint16_t opcode);
        };
    }
}

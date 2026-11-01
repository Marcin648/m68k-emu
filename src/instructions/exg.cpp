#include "instructions/exg.hpp"
#include "helpers.hpp"
#include <stdexcept>

using namespace M68K;
using namespace INSTRUCTION;

Exg::Exg(uint16_t opcode) : Instruction(opcode) {
    uint16_t dest_reg_part = (opcode >> 9) & 0x7;
    uint16_t src_reg_part = (opcode >> 0) & 0x7;
    uint16_t op_mode_part = (opcode >> 3) & 0x1F;

    switch (op_mode_part) {
        case 0x08: { // 0b01000
            this->dest_reg = get_register_type(0, dest_reg_part);
            this->src_reg = get_register_type(0, src_reg_part);
            break;
        }
        case 0x09: { // 0b01001
            this->dest_reg = get_register_type(1, dest_reg_part);
            this->src_reg = get_register_type(1, src_reg_part);
            break;
        }
        case 0x11: { // 0b10001
            this->dest_reg = get_register_type(0, dest_reg_part);
            this->src_reg = get_register_type(1, src_reg_part);
            break;
        }
        default: {
            this->is_valid = false;
        }
    }
}

void Exg::execute(CPUState& cpu_state) const {
    uint32_t pc = cpu_state.registers.get(REG_PC, SIZE_LONG);
    pc += SIZE_WORD;
    cpu_state.registers.set(REG_PC, SIZE_LONG, pc);

    uint32_t src_data = cpu_state.registers.get(this->src_reg, SIZE_LONG);
    uint32_t dest_data = cpu_state.registers.get(this->dest_reg, SIZE_LONG);
    cpu_state.registers.set(this->src_reg, SIZE_LONG, dest_data);
    cpu_state.registers.set(this->dest_reg, SIZE_LONG, src_data);
}

std::string Exg::disassembly(CPUState& cpu_state) const {
    uint32_t pc = cpu_state.registers.get(REG_PC, SIZE_LONG);
    pc += SIZE_WORD;
    cpu_state.registers.set(REG_PC, SIZE_LONG, pc);

    std::ostringstream output;
    output << "exg " << DISASSEMBLER::reg(this->dest_reg) << ", " << DISASSEMBLER::reg(this->src_reg);
    return output.str();
}

std::unique_ptr<INSTRUCTION::Instruction> Exg::create(uint16_t opcode) {
    return std::make_unique<Exg>(opcode);
}

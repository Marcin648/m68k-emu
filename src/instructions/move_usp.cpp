#include "instructions/move_usp.hpp"

using namespace M68K;
using namespace INSTRUCTION;

MoveUsp::MoveUsp(uint16_t opcode) : Instruction(opcode) {
    this->addr_reg = get_register_type(1, opcode & 0x7);
    this->to_addr_reg = (opcode >> 3) & 0x1;
}

void MoveUsp::execute(CPUState& cpu_state) const {
    if (!cpu_state.require_supervisor()) {
        return;
    }

    uint32_t pc = cpu_state.registers.get(REG_PC, SIZE_LONG);
    pc += SIZE_WORD;
    cpu_state.registers.set(REG_PC, SIZE_LONG, pc);

    if (this->to_addr_reg) {
        cpu_state.registers.set(this->addr_reg, SIZE_LONG, cpu_state.registers.get_usp());
    } else {
        cpu_state.registers.set_usp(cpu_state.registers.get(this->addr_reg, SIZE_LONG));
    }
}

std::string MoveUsp::disassembly(CPUState& cpu_state) const {
    uint32_t pc = cpu_state.registers.get(REG_PC, SIZE_LONG);
    pc += SIZE_WORD;
    cpu_state.registers.set(REG_PC, SIZE_LONG, pc);

    std::ostringstream output;
    output << "move" << DISASSEMBLER::size_suffix(SIZE_LONG) << " ";
    if (this->to_addr_reg) {
        output << "usp, " << DISASSEMBLER::reg(this->addr_reg);
    } else {
        output << DISASSEMBLER::reg(this->addr_reg) << ", usp";
    }
    return output.str();
}

std::unique_ptr<INSTRUCTION::Instruction> MoveUsp::create(uint16_t opcode) {
    return std::make_unique<MoveUsp>(opcode);
}

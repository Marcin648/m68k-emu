#include "instructions/pea.hpp"
#include "helpers.hpp"
#include <stdexcept>

using namespace M68K;
using namespace INSTRUCTION;

Pea::Pea(uint16_t opcode) : Instruction(opcode) {
    uint16_t addr_mode_part = (opcode >> 3) & 0x7;
    uint16_t addr_reg_part = (opcode >> 0) & 0x7;

    this->addr_mode = get_addressing_mode(addr_mode_part, addr_reg_part);
    this->addr_reg = get_register_type(addr_mode_part, addr_reg_part);

    this->data_size = SIZE_LONG;

    if (!IS_CONTROL_ADDR(this->addr_mode)) {
        this->is_valid = false;
    }
}

void Pea::execute(CPUState& cpu_state) const {
    uint32_t pc = cpu_state.registers.get(REG_PC, SIZE_LONG);
    pc += SIZE_WORD;
    cpu_state.registers.set(REG_PC, SIZE_LONG, pc);

    uint32_t addr = cpu_state.get_control_address(this->addr_mode, this->addr_reg, this->data_size);
    cpu_state.stack_push(SIZE_LONG, addr);
}

std::string Pea::disassembly(CPUState& cpu_state) const {
    uint32_t pc = cpu_state.registers.get(REG_PC, SIZE_LONG);
    pc += SIZE_WORD;
    cpu_state.registers.set(REG_PC, SIZE_LONG, pc);

    std::ostringstream output;
    output << "pea " << DISASSEMBLER::effective_address(this->addr_mode, this->addr_reg, this->data_size, cpu_state);
    return output.str();
}

std::unique_ptr<INSTRUCTION::Instruction> Pea::create(uint16_t opcode) {
    return std::make_unique<Pea>(opcode);
}

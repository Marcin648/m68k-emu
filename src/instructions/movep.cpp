#include "instructions/movep.hpp"
#include "helpers.hpp"
#include <stdexcept>

using namespace M68K;
using namespace INSTRUCTION;

Movep::Movep(uint16_t opcode) : Instruction(opcode) {
    uint16_t data_reg_part = (opcode >> 9) & 0x7;
    uint16_t addr_reg_part = (opcode >> 0) & 0x7;
    uint16_t data_size_part = (opcode >> 6) & 0x1;
    uint16_t direction_part = (opcode >> 7) & 0x1;

    this->data_reg = get_register_type(0, data_reg_part);
    this->addr_reg = get_register_type(1, addr_reg_part);
    this->data_size = data_size_part ? SIZE_LONG : SIZE_WORD;
    this->to_memory = direction_part;
}

void Movep::execute(CPUState& cpu_state) const {
    uint32_t pc = cpu_state.registers.get(REG_PC, SIZE_LONG);
    pc += SIZE_WORD;
    cpu_state.registers.set(REG_PC, SIZE_LONG, pc);

    uint32_t addr = cpu_state.get_control_address(ADDR_MODE_INDIRECT_DISPLACEMENT, this->addr_reg, this->data_size);

    if (this->to_memory) {
        uint32_t data = cpu_state.registers.get(this->data_reg, this->data_size);
        for (int i = this->data_size - 1; i >= 0; i--) {
            cpu_state.memory.set(addr, SIZE_BYTE, data >> (i * 8));
            addr += 2;
        }
    } else {
        uint32_t data = 0;
        for (int i = 0; i < this->data_size; i++) {
            data = (data << 8) | cpu_state.memory.get(addr, SIZE_BYTE);
            addr += 2;
        }
        cpu_state.registers.set(this->data_reg, this->data_size, data);
    }
}

std::string Movep::disassembly(CPUState& cpu_state) const {
    uint32_t pc = cpu_state.registers.get(REG_PC, SIZE_LONG);
    pc += SIZE_WORD;
    cpu_state.registers.set(REG_PC, SIZE_LONG, pc);

    std::string addr_str = DISASSEMBLER::effective_address(ADDR_MODE_INDIRECT_DISPLACEMENT, this->addr_reg, this->data_size, cpu_state);

    std::ostringstream output;
    output << "movep" << DISASSEMBLER::size_suffix(this->data_size) << " ";
    if (this->to_memory) {
        output << DISASSEMBLER::reg(this->data_reg) << ", " << addr_str;
    } else {
        output << addr_str << ", " << DISASSEMBLER::reg(this->data_reg);
    }
    return output.str();
}

std::unique_ptr<INSTRUCTION::Instruction> Movep::create(uint16_t opcode) {
    return std::make_unique<Movep>(opcode);
}

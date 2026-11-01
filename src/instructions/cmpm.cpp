#include "instructions/cmpm.hpp"
#include "helpers.hpp"
#include <stdexcept>

using namespace M68K;
using namespace INSTRUCTION;

Cmpm::Cmpm(uint16_t opcode) : Instruction(opcode) {
    uint16_t dest_reg_part = (opcode >> 9) & 0x7;
    uint16_t src_reg_part = (opcode >> 0) & 0x7;
    uint16_t data_size_part = (opcode >> 6) & 0x3;

    this->dest_reg = get_register_type(1, dest_reg_part);
    this->src_reg = get_register_type(1, src_reg_part);

    switch (data_size_part) {
        case 0x00: { // 0b00
            this->data_size = SIZE_BYTE;
            break;
        }
        case 0x01: { // 0b01
            this->data_size = SIZE_WORD;
            break;
        }
        case 0x02: { // 0b10
            this->data_size = SIZE_LONG;
            break;
        }
        default: {
            this->is_valid = false;
        }
    }
}

void Cmpm::execute(CPUState& cpu_state) const {
    uint32_t pc = cpu_state.registers.get(REG_PC, SIZE_LONG);
    pc += SIZE_WORD;
    cpu_state.registers.set(REG_PC, SIZE_LONG, pc);

    uint32_t src_data = cpu_state.get_data(ADDR_MODE_INDIRECT_POSTINCREMENT, this->src_reg, this->data_size);
    uint32_t dest_data = cpu_state.get_data(ADDR_MODE_INDIRECT_POSTINCREMENT, this->dest_reg, this->data_size);
    uint64_t result = static_cast<uint64_t>(dest_data) - src_data;

    cpu_state.registers.set(SR_FLAG_NEGATIVE, IS_NEGATIVE(result, this->data_size));
    cpu_state.registers.set(SR_FLAG_ZERO, IS_ZERO(result, this->data_size));
    cpu_state.registers.set(SR_FLAG_OVERFLOW, IS_OVERFLOW_SUB(src_data, dest_data, result, this->data_size));
    cpu_state.registers.set(SR_FLAG_CARRY, IS_CARRY(result, this->data_size));
}

std::string Cmpm::disassembly(CPUState& cpu_state) const {
    uint32_t pc = cpu_state.registers.get(REG_PC, SIZE_LONG);
    pc += SIZE_WORD;
    cpu_state.registers.set(REG_PC, SIZE_LONG, pc);

    std::ostringstream output;
    output << "cmpm" << DISASSEMBLER::size_suffix(this->data_size)
           << " " << DISASSEMBLER::effective_address(ADDR_MODE_INDIRECT_POSTINCREMENT, this->src_reg, this->data_size, cpu_state)
           << ", " << DISASSEMBLER::effective_address(ADDR_MODE_INDIRECT_POSTINCREMENT, this->dest_reg, this->data_size, cpu_state);

    return output.str();
}

std::unique_ptr<INSTRUCTION::Instruction> Cmpm::create(uint16_t opcode) {
    return std::make_unique<Cmpm>(opcode);
}

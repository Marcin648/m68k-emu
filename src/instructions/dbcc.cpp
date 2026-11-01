#include "instructions/dbcc.hpp"
#include "helpers.hpp"
#include <stdexcept>

using namespace M68K;
using namespace INSTRUCTION;

Dbcc::Dbcc(uint16_t opcode) : Instruction(opcode) {
    uint16_t cond_part = (opcode >> 8) & 0xF;
    uint16_t data_reg_part = (opcode >> 0) & 0x7;

    this->condition = get_condition(cond_part);
    this->data_reg = get_register_type(0, data_reg_part);
}

void Dbcc::execute(CPUState& cpu_state) const {
    uint32_t pc = cpu_state.registers.get(REG_PC, SIZE_LONG);
    pc += SIZE_WORD;
    cpu_state.registers.set(REG_PC, SIZE_LONG, pc);

    int16_t displacement = cpu_state.get_data(ADDR_MODE_IMMEDIATE, REG_D0, SIZE_WORD);

    if (!cpu_state.check_condition(this->condition)) {
        uint16_t counter = cpu_state.registers.get(this->data_reg, SIZE_WORD) - 1;
        cpu_state.registers.set(this->data_reg, SIZE_WORD, counter);
        if (counter != 0xFFFF) {
            cpu_state.registers.set(REG_PC, SIZE_LONG, pc + displacement);
        }
    }
}

std::string Dbcc::disassembly(CPUState& cpu_state) const {
    uint32_t pc = cpu_state.registers.get(REG_PC, SIZE_LONG);
    pc += SIZE_WORD;
    cpu_state.registers.set(REG_PC, SIZE_LONG, pc);

    int16_t displacement = cpu_state.get_data(ADDR_MODE_IMMEDIATE, REG_D0, SIZE_WORD);

    std::ostringstream output;
    output << "db" << DISASSEMBLER::condition_suffix(this->condition)
           << " " << DISASSEMBLER::reg(this->data_reg)
           << ", $" << std::hex << (pc + displacement);
    return output.str();
}

std::unique_ptr<INSTRUCTION::Instruction> Dbcc::create(uint16_t opcode) {
    return std::make_unique<Dbcc>(opcode);
}

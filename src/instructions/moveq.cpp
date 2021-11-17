#include "instructions/moveq.hpp"
#include "instruction_functions.hpp"
#include "helpers.hpp"
#include <stdexcept>

using namespace M68K;
using namespace INSTRUCTION;

Moveq::Moveq(uint16_t opcode) : Instruction(opcode){
    uint16_t reg_part = (opcode >> 9) & 0x7;

    this->dest_mode = ADDR_MODE_DIRECT_DATA;
    this->dest_reg = getRegisterType(0, reg_part);
    this->imm_data = (opcode >> 0) & 0xFF;
    this->data_size = SIZE_LONG;
}

void Moveq::execute(CPUState& cpu_state){
    uint32_t pc = cpu_state.registers.get(REG_PC, SIZE_LONG);
    pc += SIZE_WORD;
    cpu_state.registers.set(REG_PC, SIZE_LONG, pc);

    cpu_state.setData(this->dest_mode, this->dest_reg, this->data_size, this->imm_data);

    cpu_state.registers.set(SR_FLAG_NEGATIVE, IS_NEGATIVE(this->imm_data, this->data_size));
    cpu_state.registers.set(SR_FLAG_ZERO, this->imm_data == 0);
    cpu_state.registers.set(SR_FLAG_OVERFLOW, false);
    cpu_state.registers.set(SR_FLAG_CARRY, false);
}

std::shared_ptr<INSTRUCTION::Instruction> Moveq::create(uint16_t opcode){
    return std::make_shared<Moveq>(opcode);
}
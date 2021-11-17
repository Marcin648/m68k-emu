#include "instructions/lea.hpp"
#include "instruction_functions.hpp"
#include "helpers.hpp"
#include <stdexcept>

using namespace M68K;
using namespace INSTRUCTION;

Lea::Lea(uint16_t opcode) : Instruction(opcode){
    uint16_t addr_reg_part = (opcode >> 9) & 0x7;
    uint16_t ea_mode_part = (opcode >> 3) & 0x7;
    uint16_t ea_reg_part = (opcode >> 0) & 0x7;

    this->dest_mode = getAddressingMode(ea_mode_part, ea_reg_part);
    this->dest_reg = getRegisterType(ea_mode_part, ea_reg_part);
    this->addr_reg = getRegisterType(1, addr_reg_part);
    this->data_size = SIZE_LONG;

    if(!IS_CONTROL_ADDR(this->dest_mode)){
        this->is_valid = false;
    }
}

void Lea::execute(CPUState& cpu_state){
    uint32_t pc = cpu_state.registers.get(REG_PC, SIZE_LONG);
    pc += SIZE_WORD;
    cpu_state.registers.set(REG_PC, SIZE_LONG, pc);

    uint32_t src_data = cpu_state.getControlAddress(this->dest_mode, this->dest_reg, this->data_size);
    cpu_state.setData(ADDR_MODE_DIRECT_ADDR, this->addr_reg, this->data_size, src_data);
}

std::shared_ptr<INSTRUCTION::Instruction> Lea::create(uint16_t opcode){
    return std::make_shared<Lea>(opcode);
}
#include "instructions/jsr.hpp"
#include "instruction_functions.hpp"
#include "helpers.hpp"
#include <stdexcept>

using namespace M68K;
using namespace INSTRUCTION;

Jsr::Jsr(uint16_t opcode) : Instruction(opcode){
    uint16_t ea_mode_part = (opcode >> 3) & 0x7;
    uint16_t ea_reg_part = (opcode >> 0) & 0x7;

    this->dest_mode = getAddressingMode(ea_mode_part, ea_reg_part);
    this->dest_reg = getRegisterType(ea_mode_part, ea_reg_part);
    this->data_size = SIZE_LONG;

    if(!IS_CONTROL_ADDR(this->dest_mode)){
        this->is_valid = false;
    }
}

void Jsr::execute(CPUState& cpu_state){
    uint32_t pc = cpu_state.registers.get(REG_PC, SIZE_LONG);
    pc += SIZE_WORD;
    cpu_state.registers.set(REG_PC, SIZE_LONG, pc);

    uint32_t dest_addr = cpu_state.getData(this->dest_mode, this->dest_reg, this->data_size);
    pc = cpu_state.registers.get(REG_PC, SIZE_LONG);

    cpu_state.stackPush(SIZE_LONG, pc);
    cpu_state.registers.set(REG_PC, SIZE_LONG, dest_addr);
}

std::shared_ptr<INSTRUCTION::Instruction> Jsr::create(uint16_t opcode){
    return std::make_shared<Jsr>(opcode);
}
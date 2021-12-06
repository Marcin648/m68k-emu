#include "instructions/mul.hpp"
#include "helpers.hpp"
#include <stdexcept>

using namespace M68K;
using namespace INSTRUCTION;

Mul::Mul(uint16_t opcode) : Instruction(opcode){
    uint16_t data_reg_part = (opcode >> 9) & 0x7;
    uint16_t ea_mode_mode = (opcode >> 3) & 0x7;
    uint16_t ea_reg_part = (opcode >> 0) & 0x7;

    this->is_signed = (opcode >> 8) & 0x1;
   
    this->dest_mode = ADDR_MODE_DIRECT_DATA;
    this->dest_reg = getRegisterType(0, data_reg_part);

    this->src_mode = getAddressingMode(ea_mode_mode, ea_reg_part);
    this->src_reg = getRegisterType(ea_mode_mode, ea_reg_part);

    this->data_size = SIZE_WORD;
}

void Mul::execute(CPUState& cpu_state){
    uint32_t pc = cpu_state.registers.get(REG_PC, SIZE_LONG);
    pc += SIZE_WORD;
    cpu_state.registers.set(REG_PC, SIZE_LONG, pc);

    uint16_t src_data = cpu_state.getData(this->src_mode, this->src_reg, this->data_size);
    uint16_t dest_data = cpu_state.getDataSilent(this->dest_mode, this->dest_reg, this->data_size);
    uint32_t result;

    if(this->is_signed){
        result = static_cast<uint32_t>(static_cast<int16_t>(src_data) * static_cast<int16_t>(dest_data));
    }else{
        result = src_data * dest_data;
    }

    cpu_state.setData(this->dest_mode, this->dest_reg, SIZE_LONG, result);

    cpu_state.registers.set(SR_FLAG_NEGATIVE, IS_NEGATIVE(result, SIZE_LONG));
    cpu_state.registers.set(SR_FLAG_ZERO, IS_ZERO(result, SIZE_LONG));
    cpu_state.registers.set(SR_FLAG_OVERFLOW, false);
    cpu_state.registers.set(SR_FLAG_CARRY, false);
}

std::shared_ptr<INSTRUCTION::Instruction> Mul::create(uint16_t opcode){
    return std::make_shared<Mul>(opcode);
}
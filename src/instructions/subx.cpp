#include "instructions/subx.hpp"
#include "instruction_functions.hpp"
#include "helpers.hpp"
#include <stdexcept>

using namespace M68K;
using namespace INSTRUCTION;

Subx::Subx(uint16_t opcode) : Instruction(opcode){
    uint16_t dest_reg_part = (opcode >> 9) & 0x7;
    uint16_t src_reg_part = (opcode >> 0) & 0x7;
    uint16_t mode_part = (opcode >> 3) & 0x1;
    uint16_t data_size_part = (opcode >> 6) & 0x3;

    this->dest_mode = mode_part == 0 ? ADDR_MODE_DIRECT_DATA : ADDR_MODE_INDIRECT_PREDECREMENT;
    this->dest_reg = getRegisterType(mode_part, dest_reg_part);

    this->src_mode = this->dest_mode;
    this->src_reg = getRegisterType(mode_part, src_reg_part);

    switch(data_size_part){
        case 0x00:{ // 0b00
            this->data_size = SIZE_BYTE;
            break;
        }
        case 0x01:{ // 0b01
            this->data_size = SIZE_WORD;
            break;
        }
        case 0x02:{ // 0b10
            this->data_size = SIZE_LONG;
            break;
        }
        
        default:{
            this->is_valid = false;
        }
    }
}

void Subx::execute(CPUState& cpu_state){
    uint32_t pc = cpu_state.registers.get(REG_PC, SIZE_LONG);
    pc += SIZE_WORD;
    cpu_state.registers.set(REG_PC, SIZE_LONG, pc);


    uint32_t extend_data = cpu_state.registers.get(SR_FLAG_EXTEND) ? 1 : 0;
    uint32_t src_data = cpu_state.getData(this->src_mode, this->src_reg, this->data_size);
    uint32_t dest_data = cpu_state.getDataSilent(this->dest_mode, this->dest_reg, this->data_size);
    uint64_t result = dest_data - src_data - extend_data;

    cpu_state.setData(this->dest_mode, this->dest_reg, this->data_size, result);

    if(!IS_ZERO(result, this->data_size)){ // change only if non zero
        cpu_state.registers.set(SR_FLAG_ZERO, false); 
    }

    cpu_state.registers.set(SR_FLAG_EXTEND, IS_CARRY(result, this->data_size));
    cpu_state.registers.set(SR_FLAG_NEGATIVE, IS_NEGATIVE(result, this->data_size));
    cpu_state.registers.set(SR_FLAG_OVERFLOW, IS_OVERFLOW(src_data, dest_data, result, this->data_size));
    cpu_state.registers.set(SR_FLAG_CARRY, IS_CARRY(result, this->data_size));
}

std::shared_ptr<INSTRUCTION::Instruction> Subx::create(uint16_t opcode){
    return std::make_shared<Subx>(opcode);
}
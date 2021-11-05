#include "instructions/move.hpp"
#include "instruction_decoder.hpp"
#include "instruction_functions.hpp"
#include "helpers.hpp"
#include <stdexcept>

using namespace M68K;
using namespace INSTRUCTION;

Move::Move(uint16_t opcode) : Instruction(opcode){
    this->dest_part_mode = (opcode >> 6) & 0x7;
    this->dest_part_reg = (opcode >> 9) & 0x7;
    this->src_part_mode = (opcode >> 3) & 0x7;
    this->src_part_reg = (opcode >> 0) & 0x7;
    
    this->mode_dest_addr = getAddressingMode(this->dest_part_mode, this->dest_part_reg);
    this->mode_src_addr = getAddressingMode(this->src_part_mode, this->src_part_reg);

    this->dest_reg = getRegisterType(this->dest_part_mode, this->dest_part_reg);
    this->src_reg = getRegisterType(this->src_part_mode, this->src_part_reg);

    if(this->mode_dest_addr == ADDR_MODE_UNKNOWN || this->mode_src_addr == ADDR_MODE_UNKNOWN){
        this->is_valid = false;
    }

    uint16_t data_size_part = (opcode >> 12) & 0x3;
    switch(data_size_part){
        case 0x01:{ // 0b01
            this->data_size = SIZE_BYTE;
            break;
        }
        case 0x02:{ // 0b10
            this->data_size = SIZE_LONG;
            break;
        }
        case 0x03:{ // 0b11
            this->data_size = SIZE_WORD;
            break;
        }
        default:{
            this->is_valid = false;
        }
    }

    //movea
    if((this->mode_dest_addr == ADDR_MODE_DIRECT_ADDR) && (this->data_size == SIZE_BYTE)){
        this->is_valid = false;
    }

}

void Move::execute(CPUState& cpu_state){
    uint32_t src_data = 0;

    src_data = getData(this->mode_src_addr, this->src_reg, this->data_size, cpu_state);
    setData(this->mode_dest_addr, this->dest_reg, this->data_size, cpu_state, src_data);

    if(!(this->mode_dest_addr == ADDR_MODE_DIRECT_ADDR)){ // if not movea
        cpu_state.registers.set(SR_FLAG_NEGATIVE, IS_NEGATIVE(src_data, this->data_size));
        cpu_state.registers.set(SR_FLAG_ZERO, src_data == 0);
        cpu_state.registers.set(SR_FLAG_OVERFLOW, false);
        cpu_state.registers.set(SR_FLAG_CARRY, false);
    }
}

std::shared_ptr<INSTRUCTION::Instruction> Move::create(uint16_t opcode){
    return std::make_shared<Move>(opcode);
}
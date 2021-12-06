#include "instructions/addq.hpp"
#include "helpers.hpp"
#include <stdexcept>

using namespace M68K;
using namespace INSTRUCTION;

Addq::Addq(uint16_t opcode) : Instruction(opcode){
    uint16_t ea_mode_part = (opcode >> 3) & 0x7;
    uint16_t ea_reg_part = (opcode >> 0) & 0x7;
    uint16_t data_size_part = (opcode >> 6) & 0x3;

    this->dest_mode = getAddressingMode(ea_mode_part, ea_reg_part);
    this->dest_reg = getRegisterType(ea_mode_part, ea_reg_part);

    this->imm_data = (opcode >> 9) & 0x7;
    if(this->imm_data == 0){
        this->imm_data = 8;
    }
    
    if(!IS_MEMORY_ALTERABLE(this->dest_mode)){
        this->is_valid = false;
    }

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

void Addq::execute(CPUState& cpu_state){
    uint32_t pc = cpu_state.registers.get(REG_PC, SIZE_LONG);
    pc += SIZE_WORD;
    cpu_state.registers.set(REG_PC, SIZE_LONG, pc);

    uint32_t src_data = this->imm_data;
    uint32_t dest_data = cpu_state.getDataSilent(this->dest_mode, this->dest_reg, this->data_size);
    uint64_t result = src_data + dest_data;

    cpu_state.setData(this->dest_mode, this->dest_reg, this->data_size, result);

    if(this->dest_mode != ADDR_MODE_DIRECT_ADDR){
        cpu_state.registers.set(SR_FLAG_EXTEND, IS_CARRY(result, this->data_size));
        cpu_state.registers.set(SR_FLAG_NEGATIVE, IS_NEGATIVE(result, this->data_size));
        cpu_state.registers.set(SR_FLAG_ZERO, IS_ZERO(result, this->data_size));
        cpu_state.registers.set(SR_FLAG_OVERFLOW, IS_OVERFLOW(src_data, dest_data, this->data_size));
        cpu_state.registers.set(SR_FLAG_CARRY, IS_CARRY(result, this->data_size));
    }
}

std::string Addq::disassembly(CPUState& cpu_state){
    uint32_t pc = cpu_state.registers.get(REG_PC, SIZE_LONG);
    pc += SIZE_WORD;
    cpu_state.registers.set(REG_PC, SIZE_LONG, pc);

    std::ostringstream output;
    output << "addq" << DISASSEMBLER::sizeSuffix(this->data_size)
           << " #$" << std::hex << this->imm_data
           << ", " << DISASSEMBLER::effectiveAddress(this->dest_mode, this->dest_reg, this->data_size, cpu_state);
               
    return output.str();
}

std::shared_ptr<INSTRUCTION::Instruction> Addq::create(uint16_t opcode){
    return std::make_shared<Addq>(opcode);
}
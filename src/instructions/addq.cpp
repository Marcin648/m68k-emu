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
    
    if(!IS_ALTERABLE(this->dest_mode)){
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

    if((this->dest_mode == ADDR_MODE_DIRECT_ADDR) && (this->data_size == SIZE_BYTE)){
        this->is_valid = false;
    }
}

void Addq::execute(CPUState& cpu_state) const{
    uint32_t pc = cpu_state.registers.get(REG_PC, SIZE_LONG);
    pc += SIZE_WORD;
    cpu_state.registers.set(REG_PC, SIZE_LONG, pc);

    uint32_t src_data = this->imm_data;

    if(this->dest_mode == ADDR_MODE_DIRECT_ADDR){ // always long, flags not affected
        uint32_t addr_data = cpu_state.registers.get(this->dest_reg, SIZE_LONG);
        cpu_state.registers.set(this->dest_reg, SIZE_LONG, addr_data + src_data);
        return;
    }

    uint32_t dest_data = cpu_state.getDataSilent(this->dest_mode, this->dest_reg, this->data_size);
    uint64_t result = static_cast<uint64_t>(dest_data) + src_data;

    cpu_state.setData(this->dest_mode, this->dest_reg, this->data_size, result);

    cpu_state.registers.set(SR_FLAG_EXTEND, IS_CARRY(result, this->data_size));
    cpu_state.registers.set(SR_FLAG_NEGATIVE, IS_NEGATIVE(result, this->data_size));
    cpu_state.registers.set(SR_FLAG_ZERO, IS_ZERO(result, this->data_size));
    cpu_state.registers.set(SR_FLAG_OVERFLOW, IS_OVERFLOW_ADD(src_data, dest_data, result, this->data_size));
    cpu_state.registers.set(SR_FLAG_CARRY, IS_CARRY(result, this->data_size));
}

std::string Addq::disassembly(CPUState& cpu_state) const{
    uint32_t pc = cpu_state.registers.get(REG_PC, SIZE_LONG);
    pc += SIZE_WORD;
    cpu_state.registers.set(REG_PC, SIZE_LONG, pc);

    std::ostringstream output;
    output << "addq" << DISASSEMBLER::sizeSuffix(this->data_size)
           << " #$" << std::hex << (uint32_t)this->imm_data
           << ", " << DISASSEMBLER::effectiveAddress(this->dest_mode, this->dest_reg, this->data_size, cpu_state);
               
    return output.str();
}

std::unique_ptr<INSTRUCTION::Instruction> Addq::create(uint16_t opcode){
    return std::make_unique<Addq>(opcode);
}
#include "instructions/adda.hpp"
#include "instruction_decoder.hpp"
#include "instruction_functions.hpp"
#include "helpers.hpp"
#include <stdexcept>

using namespace M68K;
using namespace INSTRUCTION;

Adda::Adda(uint16_t opcode) : Instruction(opcode){
    uint16_t addr_reg_part = (opcode >> 9) & 0x7;
    uint16_t ea_mode_mode = (opcode >> 3) & 0x7;
    uint16_t ea_reg_part = (opcode >> 0) & 0x7;
    uint16_t data_size_part = (opcode >> 8) & 0x1;
   
    this->dest_mode = ADDR_MODE_DIRECT_ADDR;
    this->dest_reg = getRegisterType(1, addr_reg_part);

    this->src_mode = getAddressingMode(ea_mode_mode, ea_reg_part);
    this->src_reg = getRegisterType(ea_mode_mode, ea_reg_part);

    switch(data_size_part){
        case 0x00:{ // 0b0
            this->data_size = SIZE_WORD;
            break;
        }
        case 0x01:{ // 0b1
            this->data_size = SIZE_LONG;
            break;
        }
        
        default:{
            this->is_valid = false;
        }
    }
}

void Adda::execute(CPUState& cpu_state){
    uint32_t pc = cpu_state.registers.get(REG_PC, SIZE_LONG);
    pc += 2;
    cpu_state.registers.set(REG_PC, SIZE_LONG, pc);

    uint32_t src_data = getData(this->src_mode, this->src_reg, this->data_size, cpu_state);
    uint32_t dest_data = getDataSilent(this->dest_mode, this->dest_reg, this->data_size, cpu_state);
    uint64_t result = src_data + dest_data;

    setData(this->dest_mode, this->dest_reg, this->data_size, cpu_state, result);
}

std::shared_ptr<INSTRUCTION::Instruction> Adda::create(uint16_t opcode){
    return std::make_shared<Adda>(opcode);
}
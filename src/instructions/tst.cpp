#include "instructions/tst.hpp"
#include "instruction_functions.hpp"
#include "helpers.hpp"
#include <stdexcept>

using namespace M68K;
using namespace INSTRUCTION;

Tst::Tst(uint16_t opcode) : Instruction(opcode){
    uint16_t ea_mode_part = (opcode >> 3) & 0x7;
    uint16_t ea_reg_part = (opcode >> 0) & 0x7;

    this->ea_mode = getAddressingMode(ea_mode_part, ea_reg_part);
    this->ea_reg = getRegisterType(ea_mode_part, ea_reg_part);

    uint16_t data_size_part = (opcode >> 6) & 0x3;
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

void Tst::execute(CPUState& cpu_state){
    uint32_t pc = cpu_state.registers.get(REG_PC, SIZE_LONG);
    pc += SIZE_WORD;
    cpu_state.registers.set(REG_PC, SIZE_LONG, pc);

    uint32_t ea_data = cpu_state.getData(this->ea_mode, this->ea_reg, this->data_size);

    cpu_state.registers.set(SR_FLAG_NEGATIVE, IS_NEGATIVE(ea_data, this->data_size));
    cpu_state.registers.set(SR_FLAG_ZERO, IS_ZERO(ea_data, this->data_size));
    cpu_state.registers.set(SR_FLAG_OVERFLOW, false);
    cpu_state.registers.set(SR_FLAG_CARRY, false);
}

std::shared_ptr<INSTRUCTION::Instruction> Tst::create(uint16_t opcode){
    return std::make_shared<Tst>(opcode);
}
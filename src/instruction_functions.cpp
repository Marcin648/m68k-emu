#include "instruction_functions.hpp"

using namespace M68K;

AddressingMode INSTRUCTION::getAddressingMode(uint16_t part_mode, uint16_t part_reg){
    AddressingMode mode = ADDR_MODE_UNKNOWN;
    if(part_mode < 0x07){
        mode = static_cast<AddressingMode>(part_mode);
    }else{
        switch(part_reg){
            case 0x00: {
                mode = ADDR_MODE_ABS_WORD;
                break;
            }
            case 0x01: {
                mode = ADDR_MODE_ABS_LONG;
                break;
            }
            case 0x02: {
                mode = ADDR_MODE_PC_DISPLACEMENT;
                break;
            }
            case 0x03: {
                mode = ADDR_MODE_PC_INDEX;
                break;
            }
            case 0x04: {
                mode = ADDR_MODE_IMMEDIATE;
                break;
            }
        }
    }
    return mode;
}

RegisterType INSTRUCTION::getRegisterType(uint16_t part_mode, uint16_t part_reg){
    RegisterType type = REG_D0;
    if(part_mode == 0){
        return static_cast<RegisterType>(part_reg);
    }else{
        return static_cast<RegisterType>(part_reg + 8);
    }
    return type;
}

Condition INSTRUCTION::getCondition(uint16_t cond_part){
    return static_cast<Condition>(cond_part & 0xF);
}
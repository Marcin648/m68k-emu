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

uint32_t INSTRUCTION::getData(AddressingMode mode, RegisterType reg, DataSize size, CPUState& state){
    uint32_t data = 0;
    switch(mode){
        case ADDR_MODE_DIRECT_ADDR:
        case ADDR_MODE_DIRECT_DATA: {
            data = state.registers.get(reg, size);
            break;
        }
    }
    return data;
}

void INSTRUCTION::setData(AddressingMode mode, RegisterType reg, DataSize size, CPUState& state, uint32_t data){
    switch (mode)
    {
        case ADDR_MODE_DIRECT_ADDR:
        case ADDR_MODE_DIRECT_DATA: {
            state.registers.set(reg, size, data);
            break;
        }
    }
}
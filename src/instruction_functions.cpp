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
        case ADDR_MODE_INDIRECT: {
            uint32_t addr = state.registers.get(reg, SIZE_LONG);
            data = state.memory.get(addr, size);
            break;
        }
        case ADDR_MODE_INDIRECT_POSTINCREMENT: {
            uint32_t addr = state.registers.get(reg, SIZE_LONG);
            data = state.memory.get(addr, size);
            addr += size;
            state.registers.set(reg, SIZE_LONG, addr);
            break;
        }
        case ADDR_MODE_INDIRECT_PREDECREMENT: {
            uint32_t addr = state.registers.get(reg, SIZE_LONG);
            addr -= size;
            data = state.memory.get(addr, size);
            state.registers.set(reg, SIZE_LONG, addr);
            break;
        }
        case ADDR_MODE_INDIRECT_DISPLACEMENT: {
            uint32_t addr = state.registers.get(reg, SIZE_LONG);
            uint32_t pc = state.registers.get(REG_PC, SIZE_LONG);
            int16_t offset = state.memory.get(pc, SIZE_WORD);
            state.registers.set(REG_PC, SIZE_LONG, pc + SIZE_WORD);
            data = state.memory.get(addr + offset, size);
            break;
        }
        case ADDR_MODE_INDIRECT_INDEX: {
            uint32_t addr = state.registers.get(reg, SIZE_LONG);
            uint32_t pc = state.registers.get(REG_PC, SIZE_LONG);
            uint16_t ext_word = state.memory.get(pc, SIZE_WORD);
            RegisterType ext_reg = getRegisterType((ext_word & 0x8000), (ext_word >> 12) & 0x7);
            DataSize ext_reg_size = ((ext_word >> 11) & 0x1) ? SIZE_LONG : SIZE_WORD;
            int8_t ext_offset = ext_word & 0xFF;
            int32_t ext_reg_offset = state.registers.get(ext_reg, ext_reg_size);

            if(ext_reg_size == SIZE_WORD){
                ext_reg_offset = static_cast<int16_t>(ext_reg_offset);
            }

            state.registers.set(REG_PC, SIZE_LONG, pc + SIZE_WORD);
            data = state.memory.get(addr + ext_reg_offset + ext_offset, size);
            break;
        }
        case ADDR_MODE_PC_DISPLACEMENT: {
            uint32_t pc = state.registers.get(REG_PC, SIZE_LONG);
            int16_t offset = state.memory.get(pc, SIZE_WORD);
            state.registers.set(REG_PC, SIZE_LONG, pc + SIZE_WORD);
            data = state.memory.get(pc + offset, size);
            break;
        }
        case ADDR_MODE_PC_INDEX: { // TODO
            uint32_t pc = state.registers.get(REG_PC, SIZE_LONG);
            uint16_t ext_word = state.memory.get(pc, SIZE_WORD);
            RegisterType ext_reg = getRegisterType((ext_word & 0x8000), (ext_word >> 12) & 0x7);
            DataSize ext_reg_size = ((ext_word >> 11) & 0x1) ? SIZE_LONG : SIZE_WORD;
            int8_t ext_offset = ext_word & 0xFF;
            int32_t ext_reg_offset = state.registers.get(ext_reg, ext_reg_size);

            if(ext_reg_size == SIZE_WORD){
                ext_reg_offset = static_cast<int16_t>(ext_reg_offset);
            }

            state.registers.set(REG_PC, SIZE_LONG, pc + SIZE_WORD);
            data = state.memory.get(pc + ext_reg_offset + ext_offset, size);
            break;
        }
        case ADDR_MODE_ABS_WORD: {
            uint32_t pc = state.registers.get(REG_PC, SIZE_LONG);
            uint32_t addr = state.memory.get(pc, SIZE_WORD);
            state.registers.set(REG_PC, SIZE_LONG, pc + SIZE_WORD);
            data = state.memory.get(addr, size);
            break;
        }
        case ADDR_MODE_ABS_LONG: {
            uint32_t pc = state.registers.get(REG_PC, SIZE_LONG);
            uint32_t addr = state.memory.get(pc, SIZE_LONG);
            state.registers.set(REG_PC, SIZE_LONG, pc + SIZE_LONG);
            data = state.memory.get(addr, size);
            break;
        }
        case ADDR_MODE_IMMEDIATE: {
            uint32_t pc = state.registers.get(REG_PC, SIZE_LONG);
            uint32_t value = state.memory.get(pc, size);
            state.registers.set(REG_PC, SIZE_LONG, pc + size);
            data = value;
            break;
        }
        case ADDR_MODE_UNKNOWN:{
            break;
        }
    }
    return data;
}

uint32_t INSTRUCTION::getDataSilent(AddressingMode mode, RegisterType reg, DataSize size, CPUState& state){
    uint32_t data = 0;
    switch(mode){
        case ADDR_MODE_DIRECT_ADDR:
        case ADDR_MODE_DIRECT_DATA: {
            data = state.registers.get(reg, size);
            break;
        }
        case ADDR_MODE_INDIRECT: {
            uint32_t addr = state.registers.get(reg, SIZE_LONG);
            data = state.memory.get(addr, size);
            break;
        }
        case ADDR_MODE_INDIRECT_POSTINCREMENT: {
            uint32_t addr = state.registers.get(reg, SIZE_LONG);
            data = state.memory.get(addr, size);
            break;
        }
        case ADDR_MODE_INDIRECT_PREDECREMENT: {
            uint32_t addr = state.registers.get(reg, SIZE_LONG);
            addr -= size;
            data = state.memory.get(addr, size);
            break;
        }
        case ADDR_MODE_INDIRECT_DISPLACEMENT: {
            uint32_t addr = state.registers.get(reg, SIZE_LONG);
            uint32_t pc = state.registers.get(REG_PC, SIZE_LONG);
            int16_t offset = state.memory.get(pc, SIZE_WORD);
            data = state.memory.get(addr + offset, size);
            break;
        }
        case ADDR_MODE_INDIRECT_INDEX: {
            uint32_t addr = state.registers.get(reg, SIZE_LONG);
            uint32_t pc = state.registers.get(REG_PC, SIZE_LONG);
            uint16_t ext_word = state.memory.get(pc, SIZE_WORD);
            RegisterType ext_reg = getRegisterType((ext_word & 0x8000), (ext_word >> 12) & 0x7);
            DataSize ext_reg_size = ((ext_word >> 11) & 0x1) ? SIZE_LONG : SIZE_WORD;
            int8_t ext_offset = ext_word & 0xFF;
            int32_t ext_reg_offset = state.registers.get(ext_reg, ext_reg_size);

            if(ext_reg_size == SIZE_WORD){
                ext_reg_offset = static_cast<int16_t>(ext_reg_offset);
            }

            data = state.memory.get(addr + ext_reg_offset + ext_offset, size);
            break;
        }
        case ADDR_MODE_PC_DISPLACEMENT: {
            uint32_t pc = state.registers.get(REG_PC, SIZE_LONG);
            int16_t offset = state.memory.get(pc, SIZE_WORD);
            data = state.memory.get(pc + offset, size);
            break;
        }
        case ADDR_MODE_PC_INDEX: { // TODO
            uint32_t pc = state.registers.get(REG_PC, SIZE_LONG);
            uint16_t ext_word = state.memory.get(pc, SIZE_WORD);
            RegisterType ext_reg = getRegisterType((ext_word & 0x8000), (ext_word >> 12) & 0x7);
            DataSize ext_reg_size = ((ext_word >> 11) & 0x1) ? SIZE_LONG : SIZE_WORD;
            int8_t ext_offset = ext_word & 0xFF;
            int32_t ext_reg_offset = state.registers.get(ext_reg, ext_reg_size);

            if(ext_reg_size == SIZE_WORD){
                ext_reg_offset = static_cast<int16_t>(ext_reg_offset);
            }

            data = state.memory.get(pc + ext_reg_offset + ext_offset, size);
            break;
        }
        case ADDR_MODE_ABS_WORD: {
            uint32_t pc = state.registers.get(REG_PC, SIZE_LONG);
            uint32_t addr = state.memory.get(pc, SIZE_WORD);
            data = state.memory.get(addr, size);
            break;
        }
        case ADDR_MODE_ABS_LONG: {
            uint32_t pc = state.registers.get(REG_PC, SIZE_LONG);
            uint32_t addr = state.memory.get(pc, SIZE_LONG);
            data = state.memory.get(addr, size);
            break;
        }
        case ADDR_MODE_IMMEDIATE: {
            uint32_t pc = state.registers.get(REG_PC, SIZE_LONG);
            uint32_t value = state.memory.get(pc, size);
            data = value;
            break;
        }
        case ADDR_MODE_UNKNOWN:{
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
        case ADDR_MODE_INDIRECT: { // todo: throw on non address register type
            uint32_t addr = state.registers.get(reg, SIZE_LONG);
            state.memory.set(addr, size, data);
            break;
        }
        case ADDR_MODE_INDIRECT_POSTINCREMENT: {
            uint32_t addr = state.registers.get(reg, SIZE_LONG);
            state.memory.set(addr, size, data);
            addr += size;
            state.registers.set(reg, SIZE_LONG, addr);
            break;
        }
        case ADDR_MODE_INDIRECT_PREDECREMENT: {
            uint32_t addr = state.registers.get(reg, SIZE_LONG);
            addr -= size;
            state.memory.set(addr, size, data);
            state.registers.set(reg, SIZE_LONG, addr);
            break;
        }
        case ADDR_MODE_INDIRECT_DISPLACEMENT: {
            uint32_t addr = state.registers.get(reg, SIZE_LONG);
            uint32_t pc = state.registers.get(REG_PC, SIZE_LONG);
            int16_t offset = state.memory.get(pc, SIZE_WORD);
            state.registers.set(REG_PC, SIZE_LONG, pc + SIZE_WORD);
            state.memory.set(addr + offset, size, data);
            break;
        }
        case ADDR_MODE_INDIRECT_INDEX: {
            uint32_t addr = state.registers.get(reg, SIZE_LONG);
            uint32_t pc = state.registers.get(REG_PC, SIZE_LONG);
            uint16_t ext_word = state.memory.get(pc, SIZE_WORD);
            RegisterType ext_reg = getRegisterType((ext_word & 0x8000), (ext_word >> 12) & 0x7);
            DataSize ext_reg_size = ((ext_word >> 11) & 0x1) ? SIZE_LONG : SIZE_WORD;
            int8_t ext_offset = ext_word & 0xFF;
            int32_t ext_reg_offset = state.registers.get(ext_reg, ext_reg_size);

            if(ext_reg_size == SIZE_WORD){
                ext_reg_offset = static_cast<int16_t>(ext_reg_offset);
            }

            state.registers.set(REG_PC, SIZE_LONG, pc + SIZE_WORD);
            state.memory.set(addr + ext_reg_offset + ext_offset, size, data);
            break;
        }
        case ADDR_MODE_PC_DISPLACEMENT: {
            uint32_t pc = state.registers.get(REG_PC, SIZE_LONG);
            int16_t offset = state.memory.get(pc, SIZE_WORD);
            state.registers.set(REG_PC, SIZE_LONG, pc + SIZE_WORD);
            state.memory.set(pc + offset, size, data);
            break;
        }
        case ADDR_MODE_PC_INDEX: { // TODO
            uint32_t pc = state.registers.get(REG_PC, SIZE_LONG);
            uint16_t ext_word = state.memory.get(pc, SIZE_WORD);
            RegisterType ext_reg = getRegisterType((ext_word & 0x8000), (ext_word >> 12) & 0x7);
            DataSize ext_reg_size = ((ext_word >> 11) & 0x1) ? SIZE_LONG : SIZE_WORD;
            int8_t ext_offset = ext_word & 0xFF;
            int32_t ext_reg_offset = state.registers.get(ext_reg, ext_reg_size);

            if(ext_reg_size == SIZE_WORD){
                ext_reg_offset = static_cast<int16_t>(ext_reg_offset);
            }

            state.registers.set(REG_PC, SIZE_LONG, pc + SIZE_WORD);
            state.memory.set(pc + ext_reg_offset + ext_offset, size, data);
            break;
        }
        case ADDR_MODE_ABS_WORD: {
            uint32_t pc = state.registers.get(REG_PC, SIZE_LONG);
            uint32_t addr = state.memory.get(pc, SIZE_WORD);
            state.registers.set(REG_PC, SIZE_LONG, pc + SIZE_WORD);
            state.memory.set(addr, size, data);
            break;
        }
        case ADDR_MODE_ABS_LONG: {
            uint32_t pc = state.registers.get(REG_PC, SIZE_LONG);
            uint32_t addr = state.memory.get(pc, SIZE_LONG);
            state.registers.set(REG_PC, SIZE_LONG, pc + SIZE_LONG);
            state.memory.set(addr, size, data);
            break;
        }
        case ADDR_MODE_IMMEDIATE:
        case ADDR_MODE_UNKNOWN:{
            break;
        }
    }
}
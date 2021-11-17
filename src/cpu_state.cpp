#include "cpu_state.hpp"
#include "instruction_functions.hpp"

using namespace M68K;

uint32_t CPUState::stackPop(DataSize size){
    uint32_t stack_ptr = this->registers.get(REG_USP, SIZE_LONG);
    uint32_t data = this->memory.get(stack_ptr, size);
    stack_ptr += static_cast<uint32_t>(size);
    this->registers.set(REG_USP, SIZE_LONG, stack_ptr);
    return data;
}

void CPUState::stackPush(DataSize size, uint32_t data){
    uint32_t stack_ptr = this->registers.get(REG_USP, SIZE_LONG);
    stack_ptr -= static_cast<uint32_t>(size);
    this->registers.set(REG_USP, SIZE_LONG, stack_ptr);
    this->memory.set(stack_ptr, size, data);
}

uint32_t CPUState::getData(AddressingMode mode, RegisterType reg, DataSize size){
    uint32_t data = 0;
    switch(mode){
        case ADDR_MODE_DIRECT_ADDR:
        case ADDR_MODE_DIRECT_DATA: {
            data = this->registers.get(reg, size);
            break;
        }
        case ADDR_MODE_INDIRECT: {
            uint32_t addr = this->registers.get(reg, SIZE_LONG);
            data = this->memory.get(addr, size);
            break;
        }
        case ADDR_MODE_INDIRECT_POSTINCREMENT: {
            uint32_t addr = this->registers.get(reg, SIZE_LONG);
            data = this->memory.get(addr, size);
            addr += size;
            this->registers.set(reg, SIZE_LONG, addr);
            break;
        }
        case ADDR_MODE_INDIRECT_PREDECREMENT: {
            uint32_t addr = this->registers.get(reg, SIZE_LONG);
            addr -= size;
            data = this->memory.get(addr, size);
            this->registers.set(reg, SIZE_LONG, addr);
            break;
        }
        case ADDR_MODE_INDIRECT_DISPLACEMENT: {
            uint32_t addr = this->registers.get(reg, SIZE_LONG);
            uint32_t pc = this->registers.get(REG_PC, SIZE_LONG);
            int16_t offset = this->memory.get(pc, SIZE_WORD);
            this->registers.set(REG_PC, SIZE_LONG, pc + SIZE_WORD);
            data = this->memory.get(addr + offset, size);
            break;
        }
        case ADDR_MODE_INDIRECT_INDEX: {
            uint32_t addr = this->registers.get(reg, SIZE_LONG);
            uint32_t pc = this->registers.get(REG_PC, SIZE_LONG);
            uint16_t ext_word = this->memory.get(pc, SIZE_WORD);
            RegisterType ext_reg = INSTRUCTION::getRegisterType((ext_word & 0x8000), (ext_word >> 12) & 0x7);
            DataSize ext_reg_size = ((ext_word >> 11) & 0x1) ? SIZE_LONG : SIZE_WORD;
            int8_t ext_offset = ext_word & 0xFF;
            int32_t ext_reg_offset = this->registers.get(ext_reg, ext_reg_size);

            if(ext_reg_size == SIZE_WORD){
                ext_reg_offset = static_cast<int16_t>(ext_reg_offset);
            }

            this->registers.set(REG_PC, SIZE_LONG, pc + SIZE_WORD);
            data = this->memory.get(addr + ext_reg_offset + ext_offset, size);
            break;
        }
        case ADDR_MODE_PC_DISPLACEMENT: {
            uint32_t pc = this->registers.get(REG_PC, SIZE_LONG);
            int16_t offset = this->memory.get(pc, SIZE_WORD);
            this->registers.set(REG_PC, SIZE_LONG, pc + SIZE_WORD);
            data = pc + offset; // this->memory.get(pc + offset, size); Why?
            break;
        }
        case ADDR_MODE_PC_INDEX: {
            uint32_t pc = this->registers.get(REG_PC, SIZE_LONG);
            uint16_t ext_word = this->memory.get(pc, SIZE_WORD);
            RegisterType ext_reg = INSTRUCTION::getRegisterType((ext_word & 0x8000), (ext_word >> 12) & 0x7);
            DataSize ext_reg_size = ((ext_word >> 11) & 0x1) ? SIZE_LONG : SIZE_WORD;
            int8_t ext_offset = ext_word & 0xFF;
            int32_t ext_reg_offset = this->registers.get(ext_reg, ext_reg_size);

            if(ext_reg_size == SIZE_WORD){
                ext_reg_offset = static_cast<int16_t>(ext_reg_offset);
            }

            this->registers.set(REG_PC, SIZE_LONG, pc + SIZE_WORD);
            data = pc + ext_reg_offset + ext_offset; // this->memory.get(pc + ext_reg_offset + ext_offset, size); Why?
            break;
        }
        case ADDR_MODE_ABS_WORD: {
            uint32_t pc = this->registers.get(REG_PC, SIZE_LONG);
            uint32_t addr = this->memory.get(pc, SIZE_WORD);
            this->registers.set(REG_PC, SIZE_LONG, pc + SIZE_WORD);
            data = this->memory.get(addr, size);
            break;
        }
        case ADDR_MODE_ABS_LONG: {
            uint32_t pc = this->registers.get(REG_PC, SIZE_LONG);
            uint32_t addr = this->memory.get(pc, SIZE_LONG);
            this->registers.set(REG_PC, SIZE_LONG, pc + SIZE_LONG);
            data = this->memory.get(addr, size);
            break;
        }
        case ADDR_MODE_IMMEDIATE: {
            uint32_t pc = this->registers.get(REG_PC, SIZE_LONG);
            uint32_t value = this->memory.get(pc, size);
            this->registers.set(REG_PC, SIZE_LONG, pc + size);
            data = value;
            break;
        }
        case ADDR_MODE_UNKNOWN:{
            break;
        }
    }
    return data;
}

uint32_t CPUState::getDataSilent(AddressingMode mode, RegisterType reg, DataSize size){
    uint32_t data = 0;
    switch(mode){
        case ADDR_MODE_DIRECT_ADDR:
        case ADDR_MODE_DIRECT_DATA: {
            data = this->registers.get(reg, size);
            break;
        }
        case ADDR_MODE_INDIRECT: {
            uint32_t addr = this->registers.get(reg, SIZE_LONG);
            data = this->memory.get(addr, size);
            break;
        }
        case ADDR_MODE_INDIRECT_POSTINCREMENT: {
            uint32_t addr = this->registers.get(reg, SIZE_LONG);
            data = this->memory.get(addr, size);
            break;
        }
        case ADDR_MODE_INDIRECT_PREDECREMENT: {
            uint32_t addr = this->registers.get(reg, SIZE_LONG);
            addr -= size;
            data = this->memory.get(addr, size);
            break;
        }
        case ADDR_MODE_INDIRECT_DISPLACEMENT: {
            uint32_t addr = this->registers.get(reg, SIZE_LONG);
            uint32_t pc = this->registers.get(REG_PC, SIZE_LONG);
            int16_t offset = this->memory.get(pc, SIZE_WORD);
            data = this->memory.get(addr + offset, size);
            break;
        }
        case ADDR_MODE_INDIRECT_INDEX: {
            uint32_t addr = this->registers.get(reg, SIZE_LONG);
            uint32_t pc = this->registers.get(REG_PC, SIZE_LONG);
            uint16_t ext_word = this->memory.get(pc, SIZE_WORD);
            RegisterType ext_reg = INSTRUCTION::getRegisterType((ext_word & 0x8000), (ext_word >> 12) & 0x7);
            DataSize ext_reg_size = ((ext_word >> 11) & 0x1) ? SIZE_LONG : SIZE_WORD;
            int8_t ext_offset = ext_word & 0xFF;
            int32_t ext_reg_offset = this->registers.get(ext_reg, ext_reg_size);

            if(ext_reg_size == SIZE_WORD){
                ext_reg_offset = static_cast<int16_t>(ext_reg_offset);
            }

            data = this->memory.get(addr + ext_reg_offset + ext_offset, size);
            break;
        }
        case ADDR_MODE_PC_DISPLACEMENT: {
            uint32_t pc = this->registers.get(REG_PC, SIZE_LONG);
            int16_t offset = this->memory.get(pc, SIZE_WORD);
            data = pc + offset; // this->memory.get(pc + offset, size); Why?
            break;
        }
        case ADDR_MODE_PC_INDEX: { // TODO
            uint32_t pc = this->registers.get(REG_PC, SIZE_LONG);
            uint16_t ext_word = this->memory.get(pc, SIZE_WORD);
            RegisterType ext_reg = INSTRUCTION::getRegisterType((ext_word & 0x8000), (ext_word >> 12) & 0x7);
            DataSize ext_reg_size = ((ext_word >> 11) & 0x1) ? SIZE_LONG : SIZE_WORD;
            int8_t ext_offset = ext_word & 0xFF;
            int32_t ext_reg_offset = this->registers.get(ext_reg, ext_reg_size);

            if(ext_reg_size == SIZE_WORD){
                ext_reg_offset = static_cast<int16_t>(ext_reg_offset);
            }

            data = this->memory.get(pc + ext_reg_offset + ext_offset, size);
            break;
        }
        case ADDR_MODE_ABS_WORD: {
            uint32_t pc = this->registers.get(REG_PC, SIZE_LONG);
            uint32_t addr = this->memory.get(pc, SIZE_WORD);
            data = this->memory.get(addr, size);
            break;
        }
        case ADDR_MODE_ABS_LONG: {
            uint32_t pc = this->registers.get(REG_PC, SIZE_LONG);
            uint32_t addr = this->memory.get(pc, SIZE_LONG);
            data = this->memory.get(addr, size);
            break;
        }
        case ADDR_MODE_IMMEDIATE: {
            uint32_t pc = this->registers.get(REG_PC, SIZE_LONG);
            uint32_t value = this->memory.get(pc, size);
            data = value;
            break;
        }
        case ADDR_MODE_UNKNOWN:{
            break;
        }
    }
    return data;
}

void CPUState::setData(AddressingMode mode, RegisterType reg, DataSize size, uint32_t data){
    switch (mode)
    {
        case ADDR_MODE_DIRECT_ADDR:
        case ADDR_MODE_DIRECT_DATA: {
            this->registers.set(reg, size, data);
            break;
        }
        case ADDR_MODE_INDIRECT: { // todo: throw on non address register type
            uint32_t addr = this->registers.get(reg, SIZE_LONG);
            this->memory.set(addr, size, data);
            break;
        }
        case ADDR_MODE_INDIRECT_POSTINCREMENT: {
            uint32_t addr = this->registers.get(reg, SIZE_LONG);
            this->memory.set(addr, size, data);
            addr += size;
            this->registers.set(reg, SIZE_LONG, addr);
            break;
        }
        case ADDR_MODE_INDIRECT_PREDECREMENT: {
            uint32_t addr = this->registers.get(reg, SIZE_LONG);
            addr -= size;
            this->memory.set(addr, size, data);
            this->registers.set(reg, SIZE_LONG, addr);
            break;
        }
        case ADDR_MODE_INDIRECT_DISPLACEMENT: {
            uint32_t addr = this->registers.get(reg, SIZE_LONG);
            uint32_t pc = this->registers.get(REG_PC, SIZE_LONG);
            int16_t offset = this->memory.get(pc, SIZE_WORD);
            this->registers.set(REG_PC, SIZE_LONG, pc + SIZE_WORD);
            this->memory.set(addr + offset, size, data);
            break;
        }
        case ADDR_MODE_INDIRECT_INDEX: {
            uint32_t addr = this->registers.get(reg, SIZE_LONG);
            uint32_t pc = this->registers.get(REG_PC, SIZE_LONG);
            uint16_t ext_word = this->memory.get(pc, SIZE_WORD);
            RegisterType ext_reg = INSTRUCTION::getRegisterType((ext_word & 0x8000), (ext_word >> 12) & 0x7);
            DataSize ext_reg_size = ((ext_word >> 11) & 0x1) ? SIZE_LONG : SIZE_WORD;
            int8_t ext_offset = ext_word & 0xFF;
            int32_t ext_reg_offset = this->registers.get(ext_reg, ext_reg_size);

            if(ext_reg_size == SIZE_WORD){
                ext_reg_offset = static_cast<int16_t>(ext_reg_offset);
            }

            this->registers.set(REG_PC, SIZE_LONG, pc + SIZE_WORD);
            this->memory.set(addr + ext_reg_offset + ext_offset, size, data);
            break;
        }
        // case ADDR_MODE_PC_DISPLACEMENT: { // read-only
        //     uint32_t pc = this->registers.get(REG_PC, SIZE_LONG);
        //     int16_t offset = this->memory.get(pc, SIZE_WORD);
        //     this->registers.set(REG_PC, SIZE_LONG, pc + SIZE_WORD);
        //     this->memory.set(pc + offset, size, data);
        //     break;
        // }
        // case ADDR_MODE_PC_INDEX: { // read-only
        //     uint32_t pc = this->registers.get(REG_PC, SIZE_LONG);
        //     uint16_t ext_word = this->memory.get(pc, SIZE_WORD);
        //     RegisterType ext_reg = INSTRUCTION::getRegisterType((ext_word & 0x8000), (ext_word >> 12) & 0x7);
        //     DataSize ext_reg_size = ((ext_word >> 11) & 0x1) ? SIZE_LONG : SIZE_WORD;
        //     int8_t ext_offset = ext_word & 0xFF;
        //     int32_t ext_reg_offset = this->registers.get(ext_reg, ext_reg_size);

        //     if(ext_reg_size == SIZE_WORD){
        //         ext_reg_offset = static_cast<int16_t>(ext_reg_offset);
        //     }

        //     this->registers.set(REG_PC, SIZE_LONG, pc + SIZE_WORD);
        //     this->memory.set(pc + ext_reg_offset + ext_offset, size, data);
        //     break;
        // }
        case ADDR_MODE_ABS_WORD: {
            uint32_t pc = this->registers.get(REG_PC, SIZE_LONG);
            uint32_t addr = this->memory.get(pc, SIZE_WORD);
            this->registers.set(REG_PC, SIZE_LONG, pc + SIZE_WORD);
            this->memory.set(addr, size, data);
            break;
        }
        case ADDR_MODE_ABS_LONG: {
            uint32_t pc = this->registers.get(REG_PC, SIZE_LONG);
            uint32_t addr = this->memory.get(pc, SIZE_LONG);
            this->registers.set(REG_PC, SIZE_LONG, pc + SIZE_LONG);
            this->memory.set(addr, size, data);
            break;
        }
        case ADDR_MODE_PC_DISPLACEMENT:
        case ADDR_MODE_PC_INDEX:
        case ADDR_MODE_IMMEDIATE:
        case ADDR_MODE_UNKNOWN:{
            break;
        }
    }
}
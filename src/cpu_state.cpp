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

uint32_t CPUState::getControlAddress(AddressingMode mode, RegisterType reg, DataSize size){
    uint32_t addr = 0;
    switch(mode){
        case ADDR_MODE_INDIRECT: {
            addr = this->registers.get(reg, SIZE_LONG);
            break;
        }
        case ADDR_MODE_INDIRECT_DISPLACEMENT: {
            addr = this->registers.get(reg, SIZE_LONG);
            uint32_t pc = this->registers.get(REG_PC, SIZE_LONG);
            int16_t offset = this->memory.get(pc, SIZE_WORD);
            this->registers.set(REG_PC, SIZE_LONG, pc + SIZE_WORD);
            addr += offset;
            break;
        }
        case ADDR_MODE_INDIRECT_INDEX: {
            addr = this->registers.get(reg, SIZE_LONG);
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
            addr += ext_reg_offset + ext_offset;
            break;
        }
        case ADDR_MODE_PC_DISPLACEMENT: {
            uint32_t pc = this->registers.get(REG_PC, SIZE_LONG);
            int16_t offset = this->memory.get(pc, SIZE_WORD);
            this->registers.set(REG_PC, SIZE_LONG, pc + SIZE_WORD);
            addr = pc + offset;
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
            addr = pc + ext_reg_offset + ext_offset;
            break;
        }
        case ADDR_MODE_ABS_WORD: {
            uint32_t pc = this->registers.get(REG_PC, SIZE_LONG);
            this->registers.set(REG_PC, SIZE_LONG, pc + SIZE_WORD);
            addr = this->memory.get(pc, SIZE_WORD);
            break;
        }
        case ADDR_MODE_ABS_LONG: {
            uint32_t pc = this->registers.get(REG_PC, SIZE_LONG);
            this->registers.set(REG_PC, SIZE_LONG, pc + SIZE_LONG);
            addr = this->memory.get(pc, SIZE_LONG);
            break;
        }
        case ADDR_MODE_IMMEDIATE:
        case ADDR_MODE_INDIRECT_POSTINCREMENT:
        case ADDR_MODE_INDIRECT_PREDECREMENT:
        case ADDR_MODE_DIRECT_ADDR:
        case ADDR_MODE_DIRECT_DATA:
        case ADDR_MODE_UNKNOWN:{
            break;
        }
    }
    return addr;
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
            data = this->memory.get(pc + offset, size);
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
            data = this->memory.get(pc + ext_reg_offset + ext_offset, size);
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
            data = this->memory.get(pc + offset, size);
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
        case ADDR_MODE_INDIRECT: {
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

bool CPUState::checkCondition(Condition cond){
    //bool flag_extend = this->registers.get(SR_FLAG_EXTEND);
    bool flag_negative = this->registers.get(SR_FLAG_NEGATIVE);
    bool flag_zero = this->registers.get(SR_FLAG_ZERO);
    bool flag_overflow = this->registers.get(SR_FLAG_OVERFLOW);
    bool flag_carry = this->registers.get(SR_FLAG_CARRY);

    switch(cond){
        case COND_TRUE: { return true; }
        case COND_FALSE: { return false; }
        case COND_HIGHER: { return !flag_carry && !flag_zero; }
        case COND_LOWER_SAME: {return flag_carry || flag_zero; }
        case COND_CARRY_CLEAR: {return !flag_carry; }
        case COND_CARRY_SET: {return flag_carry; }
        case COND_NOT_EQUAL: {return !flag_zero; }
        case COND_EQUAL: {return flag_zero; }
        case COND_OVERFLOW_CLEAR: {return !flag_overflow; }
        case COND_OVERFLOW_SET: {return flag_overflow; }
        case COND_PLUS: {return !flag_negative; }
        case COND_MINUS: {return flag_negative; }
        case COND_GREATER_EQUAL: {return flag_negative == flag_overflow; }
        case COND_LESS_THAN: {return flag_negative != flag_overflow; }
        case COND_GREATER_THAN: {return !flag_zero && (flag_negative == flag_overflow); }
        case COND_LESS_EQUAL: {return flag_zero || (flag_negative != flag_overflow); }
    }
    return false;
}
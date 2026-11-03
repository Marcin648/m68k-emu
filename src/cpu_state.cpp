#include "cpu_state.hpp"
#include "instructions/instruction.hpp"
#include "helpers.hpp"

#include <cstdio>

using namespace M68K;
using namespace INSTRUCTION;

uint32_t CPUState::stack_pop(DataSize size) {
    uint32_t stack_ptr = this->registers.get(REG_USP, SIZE_LONG);
    uint32_t data = this->memory.get(stack_ptr, size);
    stack_ptr += static_cast<uint32_t>(size);
    this->registers.set(REG_USP, SIZE_LONG, stack_ptr);
    return data;
}

void CPUState::stack_push(DataSize size, uint32_t data) {
    uint32_t stack_ptr = this->registers.get(REG_USP, SIZE_LONG);
    stack_ptr -= static_cast<uint32_t>(size);
    this->registers.set(REG_USP, SIZE_LONG, stack_ptr);
    this->memory.set(stack_ptr, size, data);
}

void CPUState::exception(uint8_t vector) {
    uint32_t sr = this->registers.get(REG_SR, SIZE_WORD);
    uint32_t pc = this->registers.get(REG_PC, SIZE_LONG);
    this->registers.set(SR_FLAG_SUPERVISOR, true);
    this->registers.set(SR_FLAG_TRACE, false);
    this->stack_push(SIZE_LONG, pc);
    this->stack_push(SIZE_WORD, sr);
    this->registers.set(REG_PC, SIZE_LONG, this->memory.get(vector * 4, SIZE_LONG));
    this->stopped = false;

    switch (vector) {
        case VECTOR_ILLEGAL:
        case VECTOR_PRIVILEGE_VIOLATION:
        case VECTOR_LINE_A:
        case VECTOR_LINE_F: {
            this->trace_pending = false;
            break;
        }
    }
}

void CPUState::address_error(bool instruction, uint16_t opcode) {
    AddressError error = this->memory.address_error;
    this->memory.address_error = AddressError();

    uint32_t sr = this->registers.get(REG_SR, SIZE_WORD);
    uint32_t pc = this->registers.get(REG_PC, SIZE_LONG);
    uint32_t function_code = ((sr & SR_FLAG_SUPERVISOR) ? 4 : 0) | (instruction ? 2 : 1);
    uint32_t status = (error.read ? 0x10 : 0) | (instruction ? 0 : 0x08) | function_code;

    this->registers.set(SR_FLAG_SUPERVISOR, true);
    this->registers.set(SR_FLAG_TRACE, false);
    this->stack_push(SIZE_LONG, pc);
    this->stack_push(SIZE_WORD, sr);
    this->stack_push(SIZE_WORD, opcode);
    this->stack_push(SIZE_LONG, error.address);
    this->stack_push(SIZE_WORD, status);
    this->registers.set(REG_PC, SIZE_LONG, this->memory.get(VECTOR_ADDRESS_ERROR * 4, SIZE_LONG));
    this->stopped = false;
    this->trace_pending = false;

    if (this->memory.address_error.pending) {
        this->memory.address_error = AddressError();
        this->halted = true;
    }
}

bool CPUState::require_supervisor() {
    if (this->registers.get(SR_FLAG_SUPERVISOR)) {
        return true;
    }
    this->exception(VECTOR_PRIVILEGE_VIOLATION);
    return false;
}

uint32_t CPUState::get_control_address(AddressingMode mode, RegisterType reg, DataSize size) {
    uint32_t addr = 0;
    switch (mode) {
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
            RegisterType ext_reg = Instruction::get_register_type((ext_word & 0x8000), (ext_word >> 12) & 0x7);
            DataSize ext_reg_size = ((ext_word >> 11) & 0x1) ? SIZE_LONG : SIZE_WORD;
            int8_t ext_offset = ext_word & 0xFF;
            int32_t ext_reg_offset = this->registers.get(ext_reg, ext_reg_size);

            if (ext_reg_size == SIZE_WORD) {
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
            RegisterType ext_reg = Instruction::get_register_type((ext_word & 0x8000), (ext_word >> 12) & 0x7);
            DataSize ext_reg_size = ((ext_word >> 11) & 0x1) ? SIZE_LONG : SIZE_WORD;
            int8_t ext_offset = ext_word & 0xFF;
            int32_t ext_reg_offset = this->registers.get(ext_reg, ext_reg_size);

            if (ext_reg_size == SIZE_WORD) {
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
        case ADDR_MODE_UNKNOWN: {
            break;
        }
    }
    return addr;
}

uint32_t CPUState::get_data(AddressingMode mode, RegisterType reg, DataSize size) {
    uint32_t data = 0;
    switch (mode) {
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
            RegisterType ext_reg = Instruction::get_register_type((ext_word & 0x8000), (ext_word >> 12) & 0x7);
            DataSize ext_reg_size = ((ext_word >> 11) & 0x1) ? SIZE_LONG : SIZE_WORD;
            int8_t ext_offset = ext_word & 0xFF;
            int32_t ext_reg_offset = this->registers.get(ext_reg, ext_reg_size);

            if (ext_reg_size == SIZE_WORD) {
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
            RegisterType ext_reg = Instruction::get_register_type((ext_word & 0x8000), (ext_word >> 12) & 0x7);
            DataSize ext_reg_size = ((ext_word >> 11) & 0x1) ? SIZE_LONG : SIZE_WORD;
            int8_t ext_offset = ext_word & 0xFF;
            int32_t ext_reg_offset = this->registers.get(ext_reg, ext_reg_size);

            if (ext_reg_size == SIZE_WORD) {
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
            DataSize memory_size = (size == SIZE_LONG ? SIZE_LONG : SIZE_WORD);
            uint32_t pc = this->registers.get(REG_PC, SIZE_LONG);
            uint32_t value = this->memory.get(pc, memory_size);
            this->registers.set(REG_PC, SIZE_LONG, pc + memory_size);
            data = (size == SIZE_BYTE ? MASK_8(value) : value);
            break;
        }
        case ADDR_MODE_UNKNOWN: {
            break;
        }
    }
    return data;
}

uint32_t CPUState::get_data_silent(AddressingMode mode, RegisterType reg, DataSize size) {
    uint32_t data = 0;
    switch (mode) {
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
            RegisterType ext_reg = Instruction::get_register_type((ext_word & 0x8000), (ext_word >> 12) & 0x7);
            DataSize ext_reg_size = ((ext_word >> 11) & 0x1) ? SIZE_LONG : SIZE_WORD;
            int8_t ext_offset = ext_word & 0xFF;
            int32_t ext_reg_offset = this->registers.get(ext_reg, ext_reg_size);

            if (ext_reg_size == SIZE_WORD) {
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
            RegisterType ext_reg = Instruction::get_register_type((ext_word & 0x8000), (ext_word >> 12) & 0x7);
            DataSize ext_reg_size = ((ext_word >> 11) & 0x1) ? SIZE_LONG : SIZE_WORD;
            int8_t ext_offset = ext_word & 0xFF;
            int32_t ext_reg_offset = this->registers.get(ext_reg, ext_reg_size);

            if (ext_reg_size == SIZE_WORD) {
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
            DataSize memory_size = (size == SIZE_LONG ? SIZE_LONG : SIZE_WORD);
            uint32_t pc = this->registers.get(REG_PC, SIZE_LONG);
            uint32_t value = this->memory.get(pc, memory_size);
            data = (size == SIZE_BYTE ? MASK_8(value) : value);
            break;
        }
        case ADDR_MODE_UNKNOWN: {
            break;
        }
    }
    return data;
}

void CPUState::set_data(AddressingMode mode, RegisterType reg, DataSize size, uint32_t data) {
    switch (mode) {
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
            RegisterType ext_reg = Instruction::get_register_type((ext_word & 0x8000), (ext_word >> 12) & 0x7);
            DataSize ext_reg_size = ((ext_word >> 11) & 0x1) ? SIZE_LONG : SIZE_WORD;
            int8_t ext_offset = ext_word & 0xFF;
            int32_t ext_reg_offset = this->registers.get(ext_reg, ext_reg_size);

            if (ext_reg_size == SIZE_WORD) {
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
        //     RegisterType ext_reg = Instruction::get_register_type((ext_word & 0x8000), (ext_word >> 12) & 0x7);
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
        case ADDR_MODE_UNKNOWN: {
            break;
        }
    }
}

bool CPUState::check_condition(Condition cond) {
    //bool flag_extend = this->registers.get(SR_FLAG_EXTEND);
    bool flag_negative = this->registers.get(SR_FLAG_NEGATIVE);
    bool flag_zero = this->registers.get(SR_FLAG_ZERO);
    bool flag_overflow = this->registers.get(SR_FLAG_OVERFLOW);
    bool flag_carry = this->registers.get(SR_FLAG_CARRY);

    switch (cond) {
        case COND_TRUE: return true;
        case COND_FALSE: return false;
        case COND_HIGHER: return !flag_carry && !flag_zero;
        case COND_LOWER_SAME: return flag_carry || flag_zero;
        case COND_CARRY_CLEAR: return !flag_carry;
        case COND_CARRY_SET: return flag_carry;
        case COND_NOT_EQUAL: return !flag_zero;
        case COND_EQUAL: return flag_zero;
        case COND_OVERFLOW_CLEAR: return !flag_overflow;
        case COND_OVERFLOW_SET: return flag_overflow;
        case COND_PLUS: return !flag_negative;
        case COND_MINUS: return flag_negative;
        case COND_GREATER_EQUAL: return (flag_negative && flag_overflow) || (!flag_negative && !flag_overflow);
        case COND_LESS_THAN: return (flag_negative && !flag_overflow) || (!flag_negative && flag_overflow);
        case COND_GREATER_THAN: return (flag_negative && flag_overflow && !flag_zero) || (!flag_negative && !flag_overflow && !flag_zero);
        case COND_LESS_EQUAL: return (flag_negative && !flag_overflow) || ((!flag_negative && flag_overflow) || flag_zero);
    }
    return false;
}

void CPUState::debug_print() {
    puts("CPU state:");
    for (size_t i = 0; i < 8; i++) {
        printf("D%zu: 0x%08X\n", i, this->registers.get(static_cast<RegisterType>(i), SIZE_LONG));
    }
    for (size_t i = 0; i < 8; i++) {
        printf("A%zu: 0x%08X\n", i, this->registers.get(static_cast<RegisterType>(i + 8), SIZE_LONG));
    }
    printf("USP: 0x%08X\n", this->registers.get_usp());
    printf("SSP: 0x%08X\n", this->registers.get_ssp());
    printf("PC: 0x%08X\n", this->registers.get(REG_PC, SIZE_LONG));
    printf("SR: 0x%04X\n", this->registers.get(REG_SR, SIZE_WORD));

    printf("E: %d\n", this->registers.get(SR_FLAG_EXTEND));
    printf("N: %d\n", this->registers.get(SR_FLAG_NEGATIVE));
    printf("Z: %d\n", this->registers.get(SR_FLAG_ZERO));
    printf("O: %d\n", this->registers.get(SR_FLAG_OVERFLOW));
    printf("C: %d\n", this->registers.get(SR_FLAG_CARRY));
}

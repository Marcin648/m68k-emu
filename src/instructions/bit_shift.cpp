#include "instructions/bit_shift.hpp"
#include "helpers.hpp"
#include <stdexcept>

using namespace M68K;
using namespace INSTRUCTION;
/*
    FIX ME
    SR FLAGS IS NOT WORKING CORRECTLY
*/
BitShift::BitShift(uint16_t opcode) : Instruction(opcode){
    uint16_t addr_mode_part = (opcode >> 3) & 0x7;
    uint16_t addr_reg_part = (opcode >> 0) & 0x7;
    uint16_t direction_part = (opcode >> 8) & 0x1;
    uint16_t data_size_part = (opcode >> 6) & 0x3;

    this->direction_left = direction_part == 1;
    this->is_memory = false;

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
        case 0x03:{ // 0b11
            this->data_size = SIZE_WORD;
            this->is_memory = true;
            break;
        }
        default:{
            this->is_valid = false;
        }
    }

    uint16_t instruction_type_part = (opcode >> (this->is_memory ? 3 : 9)) & 0x3;
    uint16_t is_imm_part = (opcode >> 5) & 0x1;
    uint16_t rotation_part = (opcode >> 9) & 0x7;

    this->is_imm = is_imm_part == 1;

    if(this->is_memory){
        this->addr_mode = getAddressingMode(addr_mode_part, addr_reg_part);
        this->addr_reg = getRegisterType(addr_mode_part, addr_reg_part);
        this->imm_shift = 1;
        if(!IS_MEMORY_ALTERABLE(this->addr_mode)){
            this->is_valid = false;
        }
    }else{
        if(this->is_imm){
            this->imm_shift = rotation_part + 1;
        }else{
            this->shift_reg = getRegisterType(0, rotation_part);
        }
        this->addr_mode = getAddressingMode(0, 0);
        this->addr_reg = getRegisterType(0, addr_reg_part);
    }
    
    this->instruction_type = static_cast<InstructionType>(instruction_type_part);
    switch (this->instruction_type)
    {
        case InstructionType::AS: {this->instruction_str = "as"; break;};
        case InstructionType::LS: {this->instruction_str = "ls"; break;};
        case InstructionType::ROX: {this->instruction_str = "rox"; break;};
        case InstructionType::RO: {this->instruction_str = "ro"; break;};
    }
    this->instruction_str += (this->direction_left ? "l" : "r");
}

void BitShift::execute(CPUState& cpu_state){
    uint32_t pc = cpu_state.registers.get(REG_PC, SIZE_LONG);
    pc += SIZE_WORD;
    cpu_state.registers.set(REG_PC, SIZE_LONG, pc);

    uint64_t addr_data = cpu_state.getDataSilent(this->addr_mode, this->addr_reg, this->data_size);
    uint64_t shift = this->imm_shift;
    uint64_t result = 0;
    if(!this->is_memory && !this->is_imm){
        shift = cpu_state.getData(ADDR_MODE_DIRECT_DATA, this->shift_reg, this->data_size) % 64;
    }

    switch (this->instruction_type)
    {
        case InstructionType::AS: {
            if(direction_left){
                result = addr_data << shift;
                cpu_state.registers.set(SR_FLAG_CARRY, (addr_data << shift) & 1);
                cpu_state.registers.set(SR_FLAG_EXTEND, (addr_data << shift) & 1);
            }else{
                if(IS_NEGATIVE(addr_data, this->data_size)){
                    result = addr_data >> shift | ~(~static_cast<uint64_t>(0) >> shift);
                }else{
                    result = addr_data >> shift;
                }
                cpu_state.registers.set(SR_FLAG_CARRY, (addr_data >> (shift - 1)) & 1);
                cpu_state.registers.set(SR_FLAG_EXTEND, (addr_data >> (shift - 1)) & 1);
            }
            break;
        }
        case InstructionType::LS: {
            if(direction_left){
                result = addr_data << shift;
                cpu_state.registers.set(SR_FLAG_CARRY, (addr_data << shift) & 1);
                cpu_state.registers.set(SR_FLAG_EXTEND, (addr_data << shift) & 1);
            }else{
                result = addr_data >> shift;
                cpu_state.registers.set(SR_FLAG_CARRY, (addr_data >> (shift - 1)) & 1);
                cpu_state.registers.set(SR_FLAG_EXTEND, (addr_data >> (shift - 1)) & 1);
            }
            break;
        }
        case InstructionType::ROX: {
            uint64_t carry = cpu_state.registers.get(SR_FLAG_CARRY) ? 1 : 0;
            if(direction_left){
                switch(this->data_size){
                    case SIZE_BYTE:{
                        result = addr_data | (carry << 8);
                        result = ROL_9(result, shift);
                        cpu_state.registers.set(SR_FLAG_CARRY, (result));
                        cpu_state.registers.set(SR_FLAG_EXTEND, (result));
                        break;
                    }
                    case SIZE_WORD: {
                        result = addr_data | (carry << 16);
                        result = ROL_17(result, shift);
                        cpu_state.registers.set(SR_FLAG_CARRY, (result >> 8));
                        cpu_state.registers.set(SR_FLAG_EXTEND, (result >> 8));
                        break;
                    }
                    case SIZE_LONG: {
                        result = addr_data | (carry << 32);
                        result = ROL_33(result, shift);
                        cpu_state.registers.set(SR_FLAG_CARRY, (result >> 24));
                        cpu_state.registers.set(SR_FLAG_EXTEND, (result >> 24));
                        break;
                    }
                }
            }else{
                switch(this->data_size){
                    case SIZE_BYTE:{
                        result = result | (carry << 8);
                        result = ROR_9(result, shift);
                        cpu_state.registers.set(SR_FLAG_CARRY, (result));
                        cpu_state.registers.set(SR_FLAG_EXTEND, (result));
                        break;
                    }
                    case SIZE_WORD: {
                        result = result | (carry << 16);
                        result = ROR_17(result, shift);
                        cpu_state.registers.set(SR_FLAG_CARRY, (result >> 8));
                        cpu_state.registers.set(SR_FLAG_EXTEND, (result >> 8));
                        break;
                    }
                    case SIZE_LONG: {
                        result = result | (carry << 32);
                        result = ROR_33(result, shift);
                        cpu_state.registers.set(SR_FLAG_CARRY, (result >> 24));
                        cpu_state.registers.set(SR_FLAG_EXTEND, (result >> 24));
                        break;
                    }
                }
            }
            // cpu_state.registers.set(SR_FLAG_NEGATIVE, IS_NEGATIVE(result, this->data_size));
            // cpu_state.registers.set(SR_FLAG_ZERO, IS_ZERO(result, this->data_size));
            // cpu_state.registers.set(SR_FLAG_OVERFLOW, false);
            break;
        }
        case InstructionType::RO: {
            if(direction_left){
                result = ROL(addr_data, shift, this->data_size);
                cpu_state.registers.set(SR_FLAG_CARRY, addr_data >> (8 - shift));
            }else{
                result = ROR(addr_data, shift, this->data_size);
                cpu_state.registers.set(SR_FLAG_CARRY, addr_data << (9 - shift));
            }
            // cpu_state.registers.set(SR_FLAG_NEGATIVE, IS_NEGATIVE(result, this->data_size));
            // cpu_state.registers.set(SR_FLAG_ZERO, IS_ZERO(result, this->data_size));
            // cpu_state.registers.set(SR_FLAG_OVERFLOW, false);
            break;
        }
    }

    cpu_state.registers.set(SR_FLAG_NEGATIVE, IS_NEGATIVE(result, this->data_size));
    cpu_state.registers.set(SR_FLAG_ZERO, IS_ZERO(result, this->data_size));
    cpu_state.registers.set(SR_FLAG_OVERFLOW, false);   
}

std::string BitShift::disassembly(CPUState& cpu_state){
    uint32_t pc = cpu_state.registers.get(REG_PC, SIZE_LONG);
    pc += SIZE_WORD;
    cpu_state.registers.set(REG_PC, SIZE_LONG, pc);

    std::ostringstream output;

    output << this->instruction_str << " ";
    if(this->is_imm){
        output << " #$" << std::hex << this->imm_shift;
        output << ", " << DISASSEMBLER::effectiveAddress(this->addr_mode, this->addr_reg, this->data_size, cpu_state);
    }else{
        output << " "  << DISASSEMBLER::effectiveAddress(ADDR_MODE_DIRECT_DATA, this->shift_reg, this->data_size, cpu_state);
        output << ", " << DISASSEMBLER::effectiveAddress(this->addr_mode, this->addr_reg, this->data_size, cpu_state);
    }
               
    return output.str();
}

std::shared_ptr<INSTRUCTION::Instruction> BitShift::create(uint16_t opcode){
    return std::make_shared<BitShift>(opcode);
}
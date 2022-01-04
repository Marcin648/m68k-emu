#include "instructions/bit_manip.hpp"
#include "helpers.hpp"
#include <stdexcept>

using namespace M68K;
using namespace INSTRUCTION;

BitManip::BitManip(uint16_t opcode) : Instruction(opcode){
    uint16_t dest_mode_part = (opcode >> 3) & 0x7;
    uint16_t dest_reg_part = (opcode >> 0) & 0x7;
    uint16_t src_mode_part = (opcode >> 8) & 0x1;
    uint16_t src_reg_part = (opcode >> 9) & 0x7;
    uint16_t instruction_type_part = (opcode >> 6) & 0x3;

    this->dest_mode = getAddressingMode(dest_mode_part, dest_reg_part);
    this->dest_reg = getRegisterType(dest_mode_part, dest_reg_part);

    this->src_mode = src_mode_part ? ADDR_MODE_DIRECT_DATA : ADDR_MODE_IMMEDIATE;
    this->src_reg = getRegisterType(0, src_reg_part);

    if(!IS_DATA_ALTERABLE(this->dest_mode)){
        this->is_valid = false;
    }

    if(this->dest_mode == ADDR_MODE_DIRECT_DATA){
        this->data_size = SIZE_LONG;
        this->data_bits_modulo = 32;
    }else{
        this->data_size = SIZE_BYTE;
        this->data_bits_modulo = 8;
    }

    this->instruction_type = static_cast<InstructionType>(instruction_type_part);
    switch (this->instruction_type)
    {
        case InstructionType::BTST: {this->instruction_str = "btst"; break;};
        case InstructionType::BCHG: {this->instruction_str = "bchg"; break;};
        case InstructionType::BCLR: {this->instruction_str = "bclr"; break;};
        case InstructionType::BSET: {this->instruction_str = "bset"; break;};
    }
}

void BitManip::execute(CPUState& cpu_state){
    uint32_t pc = cpu_state.registers.get(REG_PC, SIZE_LONG);
    pc += SIZE_WORD;
    cpu_state.registers.set(REG_PC, SIZE_LONG, pc);

    uint32_t shift = cpu_state.getData(this->src_mode, this->src_reg, this->data_size) % this->data_bits_modulo;
    uint32_t dest_data = cpu_state.getDataSilent(this->dest_mode, this->dest_reg, this->data_size);
    uint64_t result = dest_data;

    bool test_bit_value = (dest_data >> shift) & 0x1;

    switch (this->instruction_type)
    {
        case InstructionType::BTST: {
            break;
        }
        case InstructionType::BCHG: {
            if(test_bit_value){
                result &= ~static_cast<uint32_t>(1 << shift);
            }else{
                result |= (1 << shift);
            }
            break;
        }
        case InstructionType::BCLR: {
            result &= ~static_cast<uint32_t>(1 << shift);
            break;
        }
        case InstructionType::BSET: {
            result |= (1 << shift);
            break;
        }
    }
    cpu_state.setData(this->dest_mode, this->dest_reg, this->data_size, result);
    cpu_state.registers.set(SR_FLAG_ZERO, !test_bit_value);
}

std::string BitManip::disassembly(CPUState& cpu_state){
    uint32_t pc = cpu_state.registers.get(REG_PC, SIZE_LONG);
    pc += SIZE_WORD;
    cpu_state.registers.set(REG_PC, SIZE_LONG, pc);

    std::ostringstream output;
    output << this->instruction_str
           << " " << DISASSEMBLER::effectiveAddress(this->src_mode, this->src_reg, this->data_size, cpu_state)
           << ", " << DISASSEMBLER::effectiveAddress(this->dest_mode, this->dest_reg, this->data_size, cpu_state);
               
    return output.str();
}

std::shared_ptr<INSTRUCTION::Instruction> BitManip::create(uint16_t opcode){
    return std::make_shared<BitManip>(opcode);
}
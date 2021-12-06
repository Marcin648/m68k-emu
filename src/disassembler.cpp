#include "disassembler.hpp"

#include "instructions/instruction.hpp"

using namespace M68K;

std::string DISASSEMBLER::reg(RegisterType reg){
    switch(reg){
        case REG_D0: { return "d0"; }
        case REG_D1: { return "d1"; }
        case REG_D2: { return "d2"; }
        case REG_D3: { return "d3"; }
        case REG_D4: { return "d4"; }
        case REG_D5: { return "d5"; }
        case REG_D6: { return "d6"; }
        case REG_D7: { return "d7"; }
        case REG_A0: { return "a0"; }
        case REG_A1: { return "a1"; }
        case REG_A2: { return "a2"; }
        case REG_A3: { return "a3"; }
        case REG_A4: { return "a4"; }
        case REG_A5: { return "a5"; }
        case REG_A6: { return "a6"; }
        case REG_A7: { return "a7"; }
        case REG_PC: { return "pc"; }
        case REG_SR: { return "sr"; }
    }
    return "unknown reg";
}

std::string DISASSEMBLER::sizeSuffix(DataSize size){
    switch(size){
        case DataSize::SIZE_BYTE: { return ".b"; };
        case DataSize::SIZE_WORD: { return ".w"; };
        case DataSize::SIZE_LONG: { return ".l"; };
    }
    return "";
}

std::string DISASSEMBLER::conditionSuffix(Condition cond){
    switch(cond){
        case COND_TRUE: { return "t"; };
        case COND_FALSE: { return "f"; };
        case COND_HIGHER: { return "hi"; };
        case COND_LOWER_SAME: { return "ls"; };
        case COND_CARRY_CLEAR: { return "cc"; };
        case COND_CARRY_SET: { return "cs"; };
        case COND_NOT_EQUAL: { return "ne"; };
        case COND_EQUAL: { return "eq"; };
        case COND_OVERFLOW_CLEAR: { return "vc"; };
        case COND_OVERFLOW_SET: { return "vs"; };
        case COND_PLUS: { return "pl"; };
        case COND_MINUS: { return "mi"; };
        case COND_GREATER_EQUAL: { return "ge"; };
        case COND_LESS_THAN: { return "lt"; };
        case COND_GREATER_THAN: { return "gt"; };
        case COND_LESS_EQUAL: { return "le"; };
    }
    return "";
}

std::string DISASSEMBLER::effectiveAddress(AddressingMode mode, RegisterType reg, DataSize size, CPUState& cpu_state){
    std::ostringstream output_stream;
    switch(mode){
        case ADDR_MODE_DIRECT_ADDR:
        case ADDR_MODE_DIRECT_DATA: {
            output_stream << DISASSEMBLER::reg(reg);
            break;
        }
        case ADDR_MODE_INDIRECT: {
            output_stream << "(" << DISASSEMBLER::reg(reg) << ")";
            break;
        }
        case ADDR_MODE_INDIRECT_POSTINCREMENT: {
            output_stream << "(" << DISASSEMBLER::reg(reg) << ")+";
            break;
        }
        case ADDR_MODE_INDIRECT_PREDECREMENT: {
            output_stream << "-(" << DISASSEMBLER::reg(reg) << ")";
            break;
        }
        case ADDR_MODE_INDIRECT_DISPLACEMENT: {
            uint32_t pc = cpu_state.registers.get(REG_PC, SIZE_LONG);
            int16_t offset = cpu_state.memory.get(pc, SIZE_WORD);
            cpu_state.registers.set(REG_PC, SIZE_LONG, pc + SIZE_WORD);
            
            output_stream << "($" << std::hex << offset << "," << DISASSEMBLER::reg(reg) << ")";
            break;
        }
        case ADDR_MODE_INDIRECT_INDEX: {
            uint32_t pc = cpu_state.registers.get(REG_PC, SIZE_LONG);
            uint16_t ext_word = cpu_state.memory.get(pc, SIZE_WORD);
            RegisterType ext_reg = INSTRUCTION::Instruction::getRegisterType((ext_word & 0x8000), (ext_word >> 12) & 0x7);
            int8_t ext_offset = ext_word & 0xFF;

            cpu_state.registers.set(REG_PC, SIZE_LONG, pc + SIZE_WORD);
            output_stream << "($" << std::hex << static_cast<int16_t>(ext_offset) << "," << DISASSEMBLER::reg(reg) << "," << DISASSEMBLER::reg(ext_reg) << ")";
            break;
        }
        case ADDR_MODE_PC_DISPLACEMENT: {
            uint32_t pc = cpu_state.registers.get(REG_PC, SIZE_LONG);
            int16_t offset = cpu_state.memory.get(pc, SIZE_WORD);

            cpu_state.registers.set(REG_PC, SIZE_LONG, pc + SIZE_WORD);
            output_stream << "($" << std::hex << (offset+2) << "," << DISASSEMBLER::reg(REG_PC) << ")";
            break;
        }
        case ADDR_MODE_PC_INDEX: {
            uint32_t pc = cpu_state.registers.get(REG_PC, SIZE_LONG);
            uint16_t ext_word = cpu_state.memory.get(pc, SIZE_WORD);
            RegisterType ext_reg = INSTRUCTION::Instruction::getRegisterType((ext_word & 0x8000), (ext_word >> 12) & 0x7);
            int8_t ext_offset = ext_word & 0xFF;

            cpu_state.registers.set(REG_PC, SIZE_LONG, pc + SIZE_WORD);
            output_stream << "($" << std::hex << (static_cast<int16_t>(ext_offset)+2) << "," << DISASSEMBLER::reg(REG_PC) << "," << DISASSEMBLER::reg(ext_reg) << ")";
            break;
        }
        case ADDR_MODE_ABS_WORD: {
            uint32_t pc = cpu_state.registers.get(REG_PC, SIZE_LONG);
            uint32_t addr = cpu_state.memory.get(pc, SIZE_WORD);

            cpu_state.registers.set(REG_PC, SIZE_LONG, pc + SIZE_WORD);
            output_stream << "($" << std::hex << addr << ").w";
            break;
        }
        case ADDR_MODE_ABS_LONG: {
            uint32_t pc = cpu_state.registers.get(REG_PC, SIZE_LONG);
            uint32_t addr = cpu_state.memory.get(pc, SIZE_LONG);

            cpu_state.registers.set(REG_PC, SIZE_LONG, pc + SIZE_LONG);
            output_stream << "($" << std::hex << addr << ").l";
            break;
        }
        case ADDR_MODE_IMMEDIATE: {
            uint32_t pc = cpu_state.registers.get(REG_PC, SIZE_LONG);
            uint32_t value = cpu_state.memory.get(pc, size);

            cpu_state.registers.set(REG_PC, SIZE_LONG, pc + size);
            output_stream << "#$" << std::hex << value;
            break;
        }
        case ADDR_MODE_UNKNOWN:{
            output_stream << "addreass mode unknown";
            break;
        }
    }
    return output_stream.str();
}
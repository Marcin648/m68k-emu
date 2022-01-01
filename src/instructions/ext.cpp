#include "instructions/ext.hpp"
#include "helpers.hpp"
#include <stdexcept>

using namespace M68K;
using namespace INSTRUCTION;

Ext::Ext(uint16_t opcode) : Instruction(opcode){
    uint16_t addr_reg_part = (opcode >> 0) & 0x7;
    uint16_t data_size_part = (opcode >> 6) & 0x1;

    this->addr_mode = ADDR_MODE_DIRECT_DATA;
    this->addr_reg = getRegisterType(0, addr_reg_part);

    this->data_size = data_size_part ? SIZE_LONG : SIZE_WORD;
}

void Ext::execute(CPUState& cpu_state){
    uint32_t pc = cpu_state.registers.get(REG_PC, SIZE_LONG);
    pc += SIZE_WORD;
    cpu_state.registers.set(REG_PC, SIZE_LONG, pc);

    uint32_t result = 0;
    if(this->data_size == SIZE_WORD){
        uint8_t data_byte = cpu_state.getData(this->addr_mode, this->addr_reg, SIZE_BYTE);
        result = static_cast<uint16_t>(static_cast<int16_t>(static_cast<int8_t>(data_byte)));
    }else{
        uint16_t data_word = cpu_state.getData(this->addr_mode, this->addr_reg, SIZE_WORD);
        result = static_cast<uint32_t>(static_cast<int32_t>(static_cast<int8_t>(data_word)));
    }
    cpu_state.setData(this->addr_mode, this->addr_reg, this->data_size, result);

    cpu_state.registers.set(SR_FLAG_NEGATIVE, IS_NEGATIVE(result, this->data_size));
    cpu_state.registers.set(SR_FLAG_ZERO, IS_ZERO(result, this->data_size));
    cpu_state.registers.set(SR_FLAG_OVERFLOW, false);
    cpu_state.registers.set(SR_FLAG_CARRY, false);
}

std::string Ext::disassembly(CPUState& cpu_state){
    uint32_t pc = cpu_state.registers.get(REG_PC, SIZE_LONG);
    pc += SIZE_WORD;
    cpu_state.registers.set(REG_PC, SIZE_LONG, pc);

    std::ostringstream output;
    output << "ext" << DISASSEMBLER::sizeSuffix(this->data_size) << DISASSEMBLER::effectiveAddress(this->addr_mode, this->addr_reg, this->data_size, cpu_state);
    return output.str();
}

std::shared_ptr<INSTRUCTION::Instruction> Ext::create(uint16_t opcode){
    return std::make_shared<Ext>(opcode);
}
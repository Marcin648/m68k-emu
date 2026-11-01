#include "instructions/moveq.hpp"
#include "helpers.hpp"
#include <stdexcept>

using namespace M68K;
using namespace INSTRUCTION;

Moveq::Moveq(uint16_t opcode) : Instruction(opcode){
    uint16_t reg_part = (opcode >> 9) & 0x7;

    this->dest_mode = ADDR_MODE_DIRECT_DATA;
    this->dest_reg = getRegisterType(0, reg_part);
    this->imm_data = (opcode >> 0) & 0xFF;
    this->data_size = SIZE_LONG;
}

void Moveq::execute(CPUState& cpu_state){
    uint32_t pc = cpu_state.registers.get(REG_PC, SIZE_LONG);
    pc += SIZE_WORD;
    cpu_state.registers.set(REG_PC, SIZE_LONG, pc);

    uint32_t data = static_cast<uint32_t>(static_cast<int32_t>(this->imm_data));
    cpu_state.setData(this->dest_mode, this->dest_reg, this->data_size, data);

    cpu_state.registers.set(SR_FLAG_NEGATIVE, IS_NEGATIVE(data, this->data_size));
    cpu_state.registers.set(SR_FLAG_ZERO, data == 0);
    cpu_state.registers.set(SR_FLAG_OVERFLOW, false);
    cpu_state.registers.set(SR_FLAG_CARRY, false);
}

std::string Moveq::disassembly(CPUState& cpu_state){
    uint32_t pc = cpu_state.registers.get(REG_PC, SIZE_LONG);
    pc += SIZE_WORD;
    cpu_state.registers.set(REG_PC, SIZE_LONG, pc);

    std::ostringstream output;
    output << "moveq"
           << " #$" << std::hex << (uint32_t)(uint8_t)this->imm_data
           << ", " << DISASSEMBLER::effectiveAddress(this->dest_mode, this->dest_reg, this->data_size, cpu_state);
               
    return output.str();
}

std::shared_ptr<INSTRUCTION::Instruction> Moveq::create(uint16_t opcode){
    return std::make_shared<Moveq>(opcode);
}
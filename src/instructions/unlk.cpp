#include "instructions/unlk.hpp"
#include "helpers.hpp"
#include <stdexcept>

using namespace M68K;
using namespace INSTRUCTION;

Unlk::Unlk(uint16_t opcode) : Instruction(opcode){
    uint16_t addr_reg_part = (opcode >> 0) & 0x7;
    this->addr_reg = getRegisterType(1, addr_reg_part);
}

void Unlk::execute(CPUState& cpu_state){
    uint32_t pc;
    uint32_t addr_data;

    pc = cpu_state.registers.get(REG_PC, SIZE_LONG);
    pc += SIZE_WORD;
    cpu_state.registers.set(REG_PC, SIZE_LONG, pc);

    addr_data = cpu_state.registers.get(this->addr_reg, SIZE_LONG);
    cpu_state.registers.set(REG_USP, SIZE_LONG, addr_data);

    addr_data = cpu_state.stackPop(SIZE_LONG);
    cpu_state.registers.set(this->addr_reg, SIZE_LONG, addr_data);
}

std::shared_ptr<INSTRUCTION::Instruction> Unlk::create(uint16_t opcode){
    return std::make_shared<Unlk>(opcode);
}
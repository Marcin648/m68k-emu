#include "instructions/link.hpp"
#include "helpers.hpp"
#include <stdexcept>

using namespace M68K;
using namespace INSTRUCTION;

Link::Link(uint16_t opcode) : Instruction(opcode){
    uint16_t addr_reg_part = (opcode >> 0) & 0x7;
    this->addr_reg = getRegisterType(1, addr_reg_part);
    this->data_size = SIZE_WORD;
}

void Link::execute(CPUState& cpu_state){
    uint32_t pc;
    uint32_t addr_data;
    uint32_t stack_ptr;
    int16_t displacement;

    pc = cpu_state.registers.get(REG_PC, SIZE_LONG);
    pc += SIZE_WORD;
    displacement = cpu_state.memory.get(pc, this->data_size);
    pc += SIZE_WORD;
    cpu_state.registers.set(REG_PC, SIZE_LONG, pc);

    addr_data = cpu_state.registers.get(this->addr_reg, SIZE_LONG);
    cpu_state.stackPush(SIZE_LONG, addr_data);

    stack_ptr = cpu_state.registers.get(REG_USP, SIZE_LONG);
    cpu_state.registers.set(this->addr_reg, SIZE_LONG, stack_ptr);
    cpu_state.registers.set(REG_USP, SIZE_LONG, stack_ptr + displacement);
}

std::string Link::disassembly(CPUState& cpu_state){
    int16_t displacement;
    uint32_t pc = cpu_state.registers.get(REG_PC, SIZE_LONG);
    pc += SIZE_WORD;
    displacement = cpu_state.memory.get(pc, this->data_size);
    pc += SIZE_WORD;
    cpu_state.registers.set(REG_PC, SIZE_LONG, pc);

    std::ostringstream output;
    output << "link " << DISASSEMBLER::reg(this->addr_reg)
           << ", $" << std::hex << displacement;
    return output.str();
}

std::shared_ptr<INSTRUCTION::Instruction> Link::create(uint16_t opcode){
    return std::make_shared<Link>(opcode);
}
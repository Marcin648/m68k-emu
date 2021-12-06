#include "instructions/rts.hpp"
#include "helpers.hpp"
#include <stdexcept>

using namespace M68K;
using namespace INSTRUCTION;

Rts::Rts(uint16_t opcode) : Instruction(opcode){
    ;
}

void Rts::execute(CPUState& cpu_state){
    uint32_t pc = cpu_state.registers.get(REG_PC, SIZE_LONG);
    pc += SIZE_WORD;
    //cpu_state.registers.set(REG_PC, SIZE_LONG, pc);

    uint32_t return_addr = cpu_state.stackPop(SIZE_LONG);
    cpu_state.registers.set(REG_PC, SIZE_LONG, return_addr);
}

std::shared_ptr<INSTRUCTION::Instruction> Rts::create(uint16_t opcode){
    return std::make_shared<Rts>(opcode);
}
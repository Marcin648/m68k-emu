#include "instructions/illegal.hpp"
#include <stdexcept>

using namespace M68K;
using namespace INSTRUCTION;

void Illegal::execute(CPUState&){
    throw std::invalid_argument("Invalid opcode");
}

std::shared_ptr<INSTRUCTION::Instruction> Illegal::create(uint16_t opcode){
    return std::make_shared<Illegal>(opcode);
}
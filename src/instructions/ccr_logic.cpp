#include "instructions/ccr_logic.hpp"
#include "helpers.hpp"
#include <stdexcept>

using namespace M68K;
using namespace INSTRUCTION;

CcrLogic::CcrLogic(uint16_t opcode) : Instruction(opcode) {
    uint16_t instruction_type_part = (opcode >> 9) & 0x7;

    switch (instruction_type_part) {
        case InstructionType::ORI: {
            this->instruction_type = InstructionType::ORI;
            this->instruction_str = "ori";
            break;
        }
        case InstructionType::ANDI: {
            this->instruction_type = InstructionType::ANDI;
            this->instruction_str = "andi";
            break;
        }
        case InstructionType::EORI: {
            this->instruction_type = InstructionType::EORI;
            this->instruction_str = "eori";
            break;
        }
        default: {
            this->is_valid = false;
        }
    }
}

void CcrLogic::execute(CPUState& cpu_state) const {
    uint32_t pc = cpu_state.registers.get(REG_PC, SIZE_LONG);
    pc += SIZE_WORD;
    cpu_state.registers.set(REG_PC, SIZE_LONG, pc);

    uint32_t src_data = cpu_state.get_data(ADDR_MODE_IMMEDIATE, REG_D0, SIZE_BYTE);
    uint32_t result = cpu_state.registers.get(REG_SR, SIZE_BYTE);

    switch (this->instruction_type) {
        case InstructionType::ORI: result |= src_data; break;
        case InstructionType::ANDI: result &= src_data; break;
        case InstructionType::EORI: result ^= src_data; break;
    }
    cpu_state.registers.set(REG_SR, SIZE_BYTE, MASK_CCR(result));
}

std::string CcrLogic::disassembly(CPUState& cpu_state) const {
    uint32_t pc = cpu_state.registers.get(REG_PC, SIZE_LONG);
    pc += SIZE_WORD;
    cpu_state.registers.set(REG_PC, SIZE_LONG, pc);

    std::ostringstream output;
    output << this->instruction_str
           << " " << DISASSEMBLER::effective_address(ADDR_MODE_IMMEDIATE, REG_D0, SIZE_BYTE, cpu_state)
           << ", ccr";

    return output.str();
}

std::unique_ptr<INSTRUCTION::Instruction> CcrLogic::create(uint16_t opcode) {
    return std::make_unique<CcrLogic>(opcode);
}

#include "instructions/scc.hpp"
#include "helpers.hpp"
#include <stdexcept>

using namespace M68K;
using namespace INSTRUCTION;

Scc::Scc(uint16_t opcode) : Instruction(opcode){
    uint16_t cond_part = (opcode >> 8) & 0xF;
    uint16_t ea_mode_part = (opcode >> 3) & 0x7;
    uint16_t ea_reg_part = (opcode >> 0) & 0x7;

    this->condition = getCondition(cond_part);
    this->dest_mode = getAddressingMode(ea_mode_part, ea_reg_part);
    this->dest_reg = getRegisterType(ea_mode_part, ea_reg_part);
    this->data_size = SIZE_BYTE;

    if(!IS_DATA_ALTERABLE(this->dest_mode)){
        this->is_valid = false;
    }
}

void Scc::execute(CPUState& cpu_state){
    uint32_t pc = cpu_state.registers.get(REG_PC, SIZE_LONG);
    pc += SIZE_WORD;
    cpu_state.registers.set(REG_PC, SIZE_LONG, pc);

    bool cond_result = cpu_state.checkCondition(this->condition);
    if(cond_result){
        cpu_state.setData(this->dest_mode, this->dest_reg, SIZE_BYTE, 0xFF);
    }else{
        cpu_state.setData(this->dest_mode, this->dest_reg, SIZE_BYTE, 0x00);
    }
}

std::string Scc::disassembly(CPUState& cpu_state){
    uint32_t pc = cpu_state.registers.get(REG_PC, SIZE_LONG);
    pc += SIZE_WORD;
    cpu_state.registers.set(REG_PC, SIZE_LONG, pc);


    std::ostringstream output;
    output << "s" << DISASSEMBLER::conditionSuffix(this->condition)
           << " " << DISASSEMBLER::effectiveAddress(this->dest_mode, this->dest_reg, SIZE_BYTE, cpu_state);
    
    return output.str();
}

std::shared_ptr<INSTRUCTION::Instruction> Scc::create(uint16_t opcode){
    return std::make_shared<Scc>(opcode);
}
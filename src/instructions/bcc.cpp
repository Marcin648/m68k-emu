#include "instructions/bcc.hpp"
#include "helpers.hpp"
#include <stdexcept>

using namespace M68K;
using namespace INSTRUCTION;

Bcc::Bcc(uint16_t opcode) : Instruction(opcode){
    uint16_t cond_part = (opcode >> 8) & 0xF;
    
    this->condition = getCondition(cond_part);
    this->displacement_8 = (opcode >> 0) & 0xFF;

    switch(this->displacement_8){
        case 0x00: { this->data_size = SIZE_WORD; break; }
        case 0xFF: { this->data_size = SIZE_LONG; break; }
        default: { this->data_size = SIZE_BYTE; break; }
    }
}

void Bcc::execute(CPUState& cpu_state){
    uint32_t pc = cpu_state.registers.get(REG_PC, SIZE_LONG);
    pc += SIZE_WORD;
    cpu_state.registers.set(REG_PC, SIZE_LONG, pc);

    int32_t displacement;

    switch(this->data_size){
        case SIZE_BYTE: {
            displacement = static_cast<int8_t>(this->displacement_8);
            break;
        }
        case SIZE_WORD: {
            displacement = static_cast<int16_t>(cpu_state.getData(ADDR_MODE_IMMEDIATE, REG_D0, this->data_size));
            break;
        }
        case SIZE_LONG: {
            displacement = static_cast<int32_t>(cpu_state.getData(ADDR_MODE_IMMEDIATE, REG_D0, this->data_size));
            break;
        }
    }

    switch(this->condition){
        case COND_TRUE: { // BRA
            pc += displacement;
            cpu_state.registers.set(REG_PC, SIZE_LONG, pc);
            break;
        }

        case COND_FALSE: { // BSR
            cpu_state.stackPush(SIZE_LONG, pc);
            pc += displacement;
            cpu_state.registers.set(REG_PC, SIZE_LONG, pc);
            break;
        }
    
        default: { //Bcc
            bool cond_result = cpu_state.checkCondition(this->condition);
            if(cond_result){
                pc += displacement;
                cpu_state.registers.set(REG_PC, SIZE_LONG, pc);
            }
        }
    }
}

std::string Bcc::disassembly(CPUState& cpu_state){
    uint32_t pc = cpu_state.registers.get(REG_PC, SIZE_LONG);
    pc += SIZE_WORD;
    cpu_state.registers.set(REG_PC, SIZE_LONG, pc);

    int32_t displacement;
    switch(this->data_size){
        case SIZE_BYTE: {
            displacement = static_cast<int8_t>(this->displacement_8);
            break;
        }
        case SIZE_WORD: {
            displacement = static_cast<int16_t>(cpu_state.getData(ADDR_MODE_IMMEDIATE, REG_D0, this->data_size));
            break;
        }
        case SIZE_LONG: {
            displacement = static_cast<int32_t>(cpu_state.getData(ADDR_MODE_IMMEDIATE, REG_D0, this->data_size));
            break;
        }
    }

    std::ostringstream output;
    switch(this->condition){
        case COND_TRUE: { // BRA
            output << "bra";
            break;
        }

        case COND_FALSE: { // BSR
            output << "bsr";
            break;
        }
    
        default: { //Bcc
            output << "b" << DISASSEMBLER::conditionSuffix(this->condition);
            break;
        }
    }

    output << " $" << (pc + displacement);
    return output.str();
}

std::shared_ptr<INSTRUCTION::Instruction> Bcc::create(uint16_t opcode){
    return std::make_shared<Bcc>(opcode);
}
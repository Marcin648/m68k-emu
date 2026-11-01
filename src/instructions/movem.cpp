#include "instructions/movem.hpp"
#include "helpers.hpp"
#include <stdexcept>

using namespace M68K;
using namespace INSTRUCTION;

namespace {
    std::string register_list(uint16_t register_mask, bool reversed) {
        std::ostringstream output;
        for (int i = 0; i < 16; i++) {
            int mask_bit = reversed ? (15 - i) : i;
            if (register_mask & (1 << mask_bit)) {
                if (output.tellp() > 0) {
                    output << "/";
                }
                output << DISASSEMBLER::reg(static_cast<RegisterType>(i));
            }
        }
        return output.str();
    }
}

Movem::Movem(uint16_t opcode) : Instruction(opcode) {
    uint16_t ea_mode_part = (opcode >> 3) & 0x7;
    uint16_t ea_reg_part = (opcode >> 0) & 0x7;
    uint16_t data_size_part = (opcode >> 6) & 0x1;
    uint16_t direction_part = (opcode >> 10) & 0x1;

    this->addr_mode = get_addressing_mode(ea_mode_part, ea_reg_part);
    this->addr_reg = get_register_type(ea_mode_part, ea_reg_part);
    this->data_size = data_size_part ? SIZE_LONG : SIZE_WORD;
    this->to_registers = direction_part;

    if (this->to_registers) {
        if (!IS_CONTROL_ADDR(this->addr_mode) && (this->addr_mode != ADDR_MODE_INDIRECT_POSTINCREMENT)) {
            this->is_valid = false;
        }
    } else {
        if (!(IS_CONTROL_ADDR(this->addr_mode) && IS_ALTERABLE(this->addr_mode)) && (this->addr_mode != ADDR_MODE_INDIRECT_PREDECREMENT)) {
            this->is_valid = false;
        }
    }
}

void Movem::execute(CPUState& cpu_state) const {
    uint32_t pc = cpu_state.registers.get(REG_PC, SIZE_LONG);
    pc += SIZE_WORD;
    uint16_t register_mask = cpu_state.memory.get(pc, SIZE_WORD);
    pc += SIZE_WORD;
    cpu_state.registers.set(REG_PC, SIZE_LONG, pc);

    if (this->addr_mode == ADDR_MODE_INDIRECT_PREDECREMENT) {
        uint32_t addr = cpu_state.registers.get(this->addr_reg, SIZE_LONG);
        for (int i = 15; i >= 0; i--) {
            if (register_mask & (1 << (15 - i))) {
                addr -= this->data_size;
                cpu_state.memory.set(addr, this->data_size, cpu_state.registers.get(static_cast<RegisterType>(i), this->data_size));
            }
        }
        cpu_state.registers.set(this->addr_reg, SIZE_LONG, addr);
        return;
    }

    uint32_t addr = 0;
    if (this->addr_mode == ADDR_MODE_INDIRECT_POSTINCREMENT) {
        addr = cpu_state.registers.get(this->addr_reg, SIZE_LONG);
    } else {
        addr = cpu_state.get_control_address(this->addr_mode, this->addr_reg, this->data_size);
    }

    for (int i = 0; i < 16; i++) {
        if (register_mask & (1 << i)) {
            RegisterType reg = static_cast<RegisterType>(i);
            if (this->to_registers) {
                uint32_t data = cpu_state.memory.get(addr, this->data_size);
                if (this->data_size == SIZE_WORD) {
                    data = static_cast<uint32_t>(static_cast<int32_t>(static_cast<int16_t>(data)));
                }
                cpu_state.registers.set(reg, SIZE_LONG, data);
            } else {
                cpu_state.memory.set(addr, this->data_size, cpu_state.registers.get(reg, this->data_size));
            }
            addr += this->data_size;
        }
    }

    if (this->addr_mode == ADDR_MODE_INDIRECT_POSTINCREMENT) {
        cpu_state.registers.set(this->addr_reg, SIZE_LONG, addr);
    }
}

std::string Movem::disassembly(CPUState& cpu_state) const {
    uint32_t pc = cpu_state.registers.get(REG_PC, SIZE_LONG);
    pc += SIZE_WORD;
    uint16_t register_mask = cpu_state.memory.get(pc, SIZE_WORD);
    pc += SIZE_WORD;
    cpu_state.registers.set(REG_PC, SIZE_LONG, pc);

    std::string list_str = register_list(register_mask, this->addr_mode == ADDR_MODE_INDIRECT_PREDECREMENT);
    std::string addr_str = DISASSEMBLER::effective_address(this->addr_mode, this->addr_reg, this->data_size, cpu_state);

    std::ostringstream output;
    output << "movem" << DISASSEMBLER::size_suffix(this->data_size) << " ";
    if (this->to_registers) {
        output << addr_str << ", " << list_str;
    } else {
        output << list_str << ", " << addr_str;
    }
    return output.str();
}

std::unique_ptr<INSTRUCTION::Instruction> Movem::create(uint16_t opcode) {
    return std::make_unique<Movem>(opcode);
}

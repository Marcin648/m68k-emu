#pragma once
#include "defines.hpp"
#include "registers.hpp"
#include "cpu_state.hpp"

namespace M68K{
    namespace INSTRUCTION{
        AddressingMode getAddressingMode(uint16_t part_mode, uint16_t part_reg);
        RegisterType getRegisterType(uint16_t part_mode, uint16_t part_reg);
        AddressingMode addrModeDowngrade(AddressingMode mode);

        uint32_t getData(AddressingMode mode, RegisterType reg, DataSize size, CPUState& state);
        void setData(AddressingMode mode, RegisterType reg, DataSize size, CPUState& state, uint32_t data);
    }
}

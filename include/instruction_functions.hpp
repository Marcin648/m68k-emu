#pragma once
#include "defines.hpp"
#include "registers.hpp"

namespace M68K{
    namespace INSTRUCTION{
        AddressingMode getAddressingMode(uint16_t part_mode, uint16_t part_reg);
        RegisterType getRegister(uint16_t part_mode, uint16_t part_reg);
    }
}

#pragma once
#include "defines.hpp"
#include "registers.hpp"
#include "cpu_state.hpp"

namespace M68K{
    namespace INSTRUCTION{
        AddressingMode getAddressingMode(uint16_t part_mode, uint16_t part_reg);
        RegisterType getRegisterType(uint16_t part_mode, uint16_t part_reg);
        Condition getCondition(uint16_t cond_part);
    }
}

#pragma once

#include <string>
#include <sstream>
#include "defines.hpp"
#include "registers.hpp"
#include "cpu_state.hpp"

namespace M68K{
    namespace DISASSEMBLER{   
        std::string reg(RegisterType reg);
        std::string sizeSuffix(DataSize size);
        std::string effectiveAddress(AddressingMode mode, RegisterType reg, DataSize size, CPUState& cpu_state);
    }
}

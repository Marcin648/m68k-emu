#pragma once

#include <string>
#include <string_view>
#include <sstream>
#include "defines.hpp"
#include "registers.hpp"
#include "cpu_state.hpp"

namespace M68K {
    namespace DISASSEMBLER {
        std::string_view reg(RegisterType reg);
        std::string_view sizeSuffix(DataSize size);
        std::string effectiveAddress(AddressingMode mode, RegisterType reg, DataSize size, CPUState& cpu_state);
        std::string_view conditionSuffix(Condition cond);
    }
}

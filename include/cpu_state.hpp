#pragma once
#include "memory.hpp"
#include "registers.hpp"

namespace M68K{
    class CPUState{
    private:
        
    public:
        CPUState() = default;
        CPUState(CPUState&) = default;

        Registers registers = Registers(); 
        Memory memory = Memory();
    };
}

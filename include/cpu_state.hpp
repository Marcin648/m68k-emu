#pragma once
#include "memory.hpp"
#include "registers.hpp"

namespace M68K{
    class CPUState{   
    public:
        CPUState() = default;

        Registers registers = Registers(); 
        Memory memory = Memory();

        uint32_t stackPop(DataSize size);
        void stackPush(DataSize size, uint32_t data);
        uint32_t getControlAddress(AddressingMode mode, RegisterType reg, DataSize size);
        uint32_t getData(AddressingMode mode, RegisterType reg, DataSize size);
        uint32_t getDataSilent(AddressingMode mode, RegisterType reg, DataSize size);
        void setData(AddressingMode mode, RegisterType reg, DataSize size, uint32_t data);
        bool checkCondition(Condition cond);

        void debugPrint();
    };
}

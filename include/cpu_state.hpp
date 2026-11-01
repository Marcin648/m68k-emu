#pragma once
#include "memory.hpp"
#include "registers.hpp"

namespace M68K {
    class CPUState {
    public:
        CPUState() = default;

        Registers registers = Registers();
        Memory memory = Memory();

        uint32_t stack_pop(DataSize size);
        void stack_push(DataSize size, uint32_t data);
        uint32_t get_control_address(AddressingMode mode, RegisterType reg, DataSize size);
        uint32_t get_data(AddressingMode mode, RegisterType reg, DataSize size);
        uint32_t get_data_silent(AddressingMode mode, RegisterType reg, DataSize size);
        void set_data(AddressingMode mode, RegisterType reg, DataSize size, uint32_t data);
        bool check_condition(Condition cond);

        void debug_print();
    };
}

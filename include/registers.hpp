#pragma once
#include <cstdint>
#include <cstddef>
#include <vector>

#include "defines.hpp"

namespace M68K{
    enum RegisterType{
        REG_D0 = 0,
        REG_D1 = 1,
        REG_D2 = 2,
        REG_D3 = 3,
        REG_D4 = 4,
        REG_D5 = 5,
        REG_D6 = 6,
        REG_D7 = 7,
        REG_A0 = 8,
        REG_A1 = 9,
        REG_A2 = 10,
        REG_A3 = 11,
        REG_A4 = 12,
        REG_A5 = 13,
        REG_A6 = 14,
        REG_A7 = 15,
        REG_USP = 15,
        REG_PC = 16,
        REG_SR = 17
    };

    enum StatusRegisterFlag{
        SR_FLAG_CARRY = (1 << 0),
        SR_FLAG_OVERFLOW = (1 << 1),
        SR_FLAG_ZERO = (1 << 2),
        SR_FLAG_NEGATIVE = (1 << 3),
        SR_FLAG_EXTEND = (1 << 4),
        SR_FLAG_SUPERVISOR = (1 << 13),
        SR_FLAG_TRACE = (1 << 15)
    };

    class Registers{
    private:
        std::vector<uint32_t> reg_buffer = std::vector<uint32_t>(REG_BUFFER_SIZE);
    public:
        static const size_t REG_BUFFER_SIZE = 18;

        Registers() = default;
        Registers(Registers&) = default;

        uint32_t get(RegisterType reg, DataSize size = DataSize::SIZE_LONG);
        void set(RegisterType reg, DataSize size, uint32_t data);

        bool get(StatusRegisterFlag flag);
        void set(StatusRegisterFlag flag, bool value);
    };
}
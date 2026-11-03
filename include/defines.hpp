#pragma once
#include <cstdint>
#include <vector>

namespace M68K {
    enum DataSize {
        SIZE_BYTE = 1,
        SIZE_WORD = 2,
        SIZE_LONG = 4
    };

    enum AddressingMode {
        ADDR_MODE_DIRECT_DATA = 0,
        ADDR_MODE_DIRECT_ADDR = 1,
        ADDR_MODE_INDIRECT = 2,
        ADDR_MODE_INDIRECT_POSTINCREMENT = 3,
        ADDR_MODE_INDIRECT_PREDECREMENT = 4,
        ADDR_MODE_INDIRECT_DISPLACEMENT = 5,
        ADDR_MODE_INDIRECT_INDEX = 6,
        ADDR_MODE_PC_DISPLACEMENT = 7,
        ADDR_MODE_PC_INDEX = 8,
        ADDR_MODE_ABS_WORD = 9,
        ADDR_MODE_ABS_LONG = 10,
        ADDR_MODE_IMMEDIATE = 11,
        ADDR_MODE_UNKNOWN = -1,
    };

    enum Condition {
        COND_TRUE = 0,
        COND_FALSE = 1,
        COND_HIGHER = 2,
        COND_LOWER_SAME = 3,
        COND_CARRY_CLEAR = 4,
        COND_CARRY_SET = 5,
        COND_NOT_EQUAL = 6,
        COND_EQUAL = 7,
        COND_OVERFLOW_CLEAR = 8,
        COND_OVERFLOW_SET = 9,
        COND_PLUS = 10,
        COND_MINUS = 11,
        COND_GREATER_EQUAL = 12,
        COND_LESS_THAN = 13,
        COND_GREATER_THAN = 14,
        COND_LESS_EQUAL = 15,
    };

    enum ExceptionVector {
        VECTOR_RESET_SSP = 0,
        VECTOR_RESET_PC = 1,
        VECTOR_BUS_ERROR = 2,
        VECTOR_ADDRESS_ERROR = 3,
        VECTOR_ILLEGAL = 4,
        VECTOR_ZERO_DIVIDE = 5,
        VECTOR_CHK = 6,
        VECTOR_TRAPV = 7,
        VECTOR_PRIVILEGE_VIOLATION = 8,
        VECTOR_TRACE = 9,
        VECTOR_LINE_A = 10,
        VECTOR_LINE_F = 11,
        VECTOR_SPURIOUS_INTERRUPT = 24,
        VECTOR_AUTOVECTOR = 24,
        VECTOR_TRAP = 32,
    };

    constexpr std::size_t MEMORY_SIZE = 0x01000000; // 16 MB
}

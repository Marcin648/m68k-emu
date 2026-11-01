#pragma once
#include "defines.hpp"

namespace M68K {
    template<typename T> inline T MSB_8(T value) { return value & 0x80; }
    template<typename T> inline T MSB_16(T value) { return value & 0x8000; }
    template<typename T> inline T MSB_32(T value) { return value & 0x80000000; }

    template<typename T> inline T MASK_8(T value) { return value & 0xff; }
    template<typename T> inline T MASK_9(T value) { return value & 0x1ff; }
    template<typename T> inline T MASK_16(T value) { return value & 0xffff; }
    template<typename T> inline T MASK_17(T value) { return value & 0x1ffff; }
    template<typename T> inline T MASK_32(T value) { return value & 0xffffffff; }
    template<typename T> inline T MASK_33(T value) { return value & 0x1ffffffff; }

    template<typename T> inline T MASK_ABOVE_8(T value) { return value & ~0xff; }
    template<typename T> inline T MASK_ABOVE_16(T value) { return value & ~0xffff; }
    template<typename T> inline T MASK_ABOVE_32(T value) { return value & ~0xffffffff; }

    template<typename T> inline T MASK_LOW_BYTE(T value) { return value & 0x0f; }
    template<typename T> inline T MASK_HIGH_BYTE(T value) { return value & 0xf0; }

    template<typename T> inline T MASK_ADDR(T value) { return value & 0x00ffffff; }

    template<typename T> inline T LSL(T a, T b) { return a << b; }
    template<typename T> inline T LSR(T a, T b) { return a >> b; }

    template<typename T> inline T ROL_8(T a, T b) { return MASK_8(LSL(a, b) | LSR(a, 8 - b)); }
    template<typename T> inline T ROL_9(T a, T b) { return MASK_9(LSL(a, b) | LSR(a, 9 - b)); }
    template<typename T> inline T ROL_16(T a, T b) { return MASK_16(LSL(a, b) | LSR(a, 16 - b)); }
    template<typename T> inline T ROL_17(T a, T b) { return MASK_17(LSL(a, b) | LSR(a, 17 - b)); }
    template<typename T> inline T ROL_32(T a, T b) { return MASK_32(LSL(a, b) | LSR(a, 32 - b)); }
    template<typename T> inline T ROL_33(T a, T b) { return MASK_33(LSL(a, b) | LSR(a, 33 - b)); }

    template<typename T> inline T ROR_8(T a, T b) { return MASK_8(LSR(a, b) | LSL(a, 8 - b)); }
    template<typename T> inline T ROR_9(T a, T b) { return MASK_9(LSR(a, b) | LSL(a, 9 - b)); }
    template<typename T> inline T ROR_16(T a, T b) { return MASK_16(LSR(a, b) | LSL(a, 16 - b)); }
    template<typename T> inline T ROR_17(T a, T b) { return MASK_17(LSR(a, b) | LSL(a, 17 - b)); }
    template<typename T> inline T ROR_32(T a, T b) { return MASK_32(LSR(a, b) | LSL(a, 32 - b)); }
    template<typename T> inline T ROR_33(T a, T b) { return MASK_33(LSR(a, b) | LSL(a, 33 - b)); }


    template<typename T> inline T MASK(T v, DataSize size) {
        switch (size) {
            case SIZE_BYTE: return MASK_8(v);
            case SIZE_WORD: return MASK_16(v);
            case SIZE_LONG: return MASK_32(v);
        }
        return 0;
    }

    template<typename T> inline T MASK_ABOVE(T v, DataSize size) {
        switch (size) {
            case SIZE_BYTE: return MASK_ABOVE_8(v);
            case SIZE_WORD: return MASK_ABOVE_16(v);
            case SIZE_LONG: return MASK_ABOVE_32(v);
        }
        return 0;
    }

    template<typename T> inline T ROL(T a, T b, DataSize size) {
        switch (size) {
            case SIZE_BYTE: return ROL_8(a, b);
            case SIZE_WORD: return ROL_16(a, b);
            case SIZE_LONG: return ROL_32(a, b);
        }
        return 0;
    }

    template<typename T> inline T ROR(T a, T b, DataSize size) {
        switch (size) {
            case SIZE_BYTE: return ROR_8(a, b);
            case SIZE_WORD: return ROR_16(a, b);
            case SIZE_LONG: return ROR_32(a, b);
        }
        return 0;
    }

    inline bool IS_ALTERABLE(AddressingMode mode) {
        return !(
            (mode == ADDR_MODE_UNKNOWN) ||
            (mode == ADDR_MODE_PC_DISPLACEMENT) ||
            (mode == ADDR_MODE_PC_INDEX) ||
            (mode == ADDR_MODE_IMMEDIATE)
        );
    }

    inline bool IS_MEMORY_ALTERABLE(AddressingMode mode) {
        return IS_ALTERABLE(mode) &&
               (mode != ADDR_MODE_DIRECT_DATA) &&
               (mode != ADDR_MODE_DIRECT_ADDR);
    }

    inline bool IS_DATA_ALTERABLE(AddressingMode mode) {
        return !(
            (mode == ADDR_MODE_UNKNOWN) ||
            (mode == ADDR_MODE_DIRECT_ADDR) ||
            (mode == ADDR_MODE_PC_DISPLACEMENT) ||
            (mode == ADDR_MODE_PC_INDEX) ||
            (mode == ADDR_MODE_IMMEDIATE)
        );
    }

    inline bool IS_CONTROL_ADDR(AddressingMode mode) {
        return !(
            (mode == ADDR_MODE_UNKNOWN) ||
            (mode == ADDR_MODE_DIRECT_DATA) ||
            (mode == ADDR_MODE_DIRECT_ADDR) ||
            (mode == ADDR_MODE_INDIRECT_POSTINCREMENT) ||
            (mode == ADDR_MODE_INDIRECT_PREDECREMENT) ||
            (mode == ADDR_MODE_IMMEDIATE)
        );
    }

    template<typename T> inline bool IS_NEGATIVE(T v, DataSize size) {
        switch (size) {
            case SIZE_BYTE: return MSB_8(v);
            case SIZE_WORD: return MSB_16(v);
            case SIZE_LONG: return MSB_32(v);
        }
        return false;
    }

    template<typename T> inline bool IS_ZERO(T v, DataSize size) {
        switch (size) {
            case SIZE_BYTE: return MASK_8(v) == 0;
            case SIZE_WORD: return MASK_16(v) == 0;
            case SIZE_LONG: return MASK_32(v) == 0;
        }
        return false;
    }

    // result = dest + src (+ X)
    inline bool IS_OVERFLOW_ADD(uint64_t src, uint64_t dest, uint64_t result, DataSize size) {
        return IS_NEGATIVE((src ^ result) & (dest ^ result), size);
    }

    // result = dest - src (- X)
    inline bool IS_OVERFLOW_SUB(uint64_t src, uint64_t dest, uint64_t result, DataSize size) {
        return IS_NEGATIVE((src ^ dest) & (dest ^ result), size);
    }

    template<typename T> inline bool IS_CARRY(T v, DataSize size) {
        switch (size) {
            case SIZE_BYTE: return v > MASK_8(v);
            case SIZE_WORD: return v > MASK_16(v);
            case SIZE_LONG: return v > MASK_32(v);
        }
        return false;
    }
}

#pragma once
#include "defines.hpp"

namespace M68K{
    template<typename T> inline T MSB_8(T value) { return value & 0x80;}
    template<typename T> inline T MSB_16(T value) { return value & 0x8000;}
    template<typename T> inline T MSB_32(T value) { return value & 0x80000000;}

    template<typename T> inline T MASK_8(T value) { return value & 0xff;}
    template<typename T> inline T MASK_16(T value) { return value & 0xffff;}
    template<typename T> inline T MASK_32(T value) { return value & 0xffffffff;}

    template<typename T> inline T MASK_ABOVE_8(T value) { return value & ~0xff;}
    template<typename T> inline T MASK_ABOVE_16(T value) { return value & ~0xffff;}
    template<typename T> inline T MASK_ABOVE_32(T value) { return value & ~0xffffffff;}

    template<typename T> inline T MASK_LOW_BYTE(T value) { return value & 0x0f;}
    template<typename T> inline T MASK_HIGH_BYTE(T value) { return value & 0xf0;}

    template<typename T> inline T MASK_ADDR(T value) { return value & 0x00ffffff;}

    template<typename T> inline T LSL(T a, T b) { return a << b;}
    template<typename T> inline T LSR(T a, T b) { return a >> b;}

    template<typename T> inline T ROL_8(T a, T b) { return MASK_8(LSL(a, b) | LSR(a, 8 - b));}
    template<typename T> inline T ROL_16(T a, T b) { return MASK_16(LSL(a, b) | LSR(a, 16 - b));}
    template<typename T> inline T ROL_32(T a, T b) { return MASK_32(LSL(a, b) | LSR(a, 32 - b));}

    template<typename T> inline T ROR_8(T a, T b) { return MASK_8(LSR(a, b) | LSL(a, 8 - b));}
    template<typename T> inline T ROR_16(T a, T b) { return MASK_16(LSR(a, b) | LSL(a, 16 - b));}
    template<typename T> inline T ROR_32(T a, T b) { return MASK_32(LSR(a, b) | LSL(a, 32 - b));}

    inline bool IS_MEMORY_ALTERABLE(AddressingMode mode) {
        return !(
            (mode == ADDR_MODE_UNKNOWN) ||
            (mode == ADDR_MODE_PC_DISPLACEMENT) ||
            (mode == ADDR_MODE_PC_INDEX) ||
            (mode == ADDR_MODE_IMMEDIATE)
        );
    }

    template<typename T> inline bool IS_NEGATIVE(T v, DataSize size) {
        switch(size){
            case SIZE_BYTE: return MSB_8(v);
            case SIZE_WORD: return MSB_16(v);
            case SIZE_LONG: return MSB_32(v);
        }
        return false;
    }

    template<typename T> inline bool IS_ZERO(T v, DataSize size) {
        switch(size){
            case SIZE_BYTE: return MASK_8(v) == 0;
            case SIZE_WORD: return MASK_16(v) == 0;
            case SIZE_LONG: return MASK_32(v) == 0;
        }
        return false;
    }

    template<typename T, typename U> inline bool IS_OVERFLOW(T a, T b, U result, DataSize size) {
        switch(size){
            case SIZE_BYTE: return (MSB_8(a) == MSB_8(b)) && (MSB_8(a) != MSB_8(result));
            case SIZE_WORD: return (MSB_16(a) == MSB_16(b)) && (MSB_8(a) != MSB_16(result));
            case SIZE_LONG: return (MSB_32(a) == MSB_32(b)) && (MSB_8(a) != MSB_32(result));
        }
        return false;
    }

    template<typename T> inline bool IS_CARRY(T v, DataSize size) {
        switch(size){
            case SIZE_BYTE: return v > MASK_8(v);
            case SIZE_WORD: return v > MASK_16(v);
            case SIZE_LONG: return v > MASK_32(v);
        }
        return false;
    }
}

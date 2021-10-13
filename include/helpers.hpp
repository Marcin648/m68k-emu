#pragma once

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

}

#pragma once
#include <catch2/catch_test_macros.hpp>
#include "m68k.hpp"

inline void checkFlags(M68K::CPUState& state, bool x, bool n, bool z, bool v, bool c) {
    CHECK(state.registers.get(M68K::SR_FLAG_EXTEND) == x);
    CHECK(state.registers.get(M68K::SR_FLAG_NEGATIVE) == n);
    CHECK(state.registers.get(M68K::SR_FLAG_ZERO) == z);
    CHECK(state.registers.get(M68K::SR_FLAG_OVERFLOW) == v);
    CHECK(state.registers.get(M68K::SR_FLAG_CARRY) == c);
}

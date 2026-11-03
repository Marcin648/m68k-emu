#pragma once
#include <catch2/catch_test_macros.hpp>
#include "m68k.hpp"

inline void check_flags(M68K::CPUState& state, bool x, bool n, bool z, bool v, bool c) {
    CHECK(state.registers.get(M68K::SR_FLAG_EXTEND) == x);
    CHECK(state.registers.get(M68K::SR_FLAG_NEGATIVE) == n);
    CHECK(state.registers.get(M68K::SR_FLAG_ZERO) == z);
    CHECK(state.registers.get(M68K::SR_FLAG_OVERFLOW) == v);
    CHECK(state.registers.get(M68K::SR_FLAG_CARRY) == c);
}

inline void check_privilege_violation(uint16_t opcode) {
    M68K::CPU cpu = M68K::CPU();
    cpu.state.registers.set_ssp(0x1000);
    cpu.state.registers.set(M68K::REG_SR, M68K::SIZE_WORD, 0x0000);
    cpu.state.registers.set(M68K::REG_A7, M68K::SIZE_LONG, 0x2000);
    cpu.state.registers.set(M68K::REG_PC, M68K::SIZE_LONG, 0x600);
    cpu.state.memory.set(0x600, M68K::SIZE_WORD, opcode);
    cpu.state.memory.set(0x602, M68K::SIZE_WORD, 0x2700);
    cpu.state.memory.set(M68K::VECTOR_PRIVILEGE_VIOLATION * 4, M68K::SIZE_LONG, 0x4000);

    cpu.step();
    CHECK(cpu.state.registers.get(M68K::REG_PC, M68K::SIZE_LONG) == 0x4000);
    CHECK(cpu.state.registers.get(M68K::REG_SR, M68K::SIZE_WORD) == 0x2000);
    CHECK(cpu.state.registers.get_usp() == 0x2000);
    CHECK(cpu.state.registers.get_ssp() == 0x0FFA);
    CHECK(cpu.state.memory.get(0x0FFA, M68K::SIZE_WORD) == 0x0000);
    CHECK(cpu.state.memory.get(0x0FFC, M68K::SIZE_LONG) == 0x600);
}

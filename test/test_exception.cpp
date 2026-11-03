#include <catch2/catch_test_macros.hpp>

#include "m68k.hpp"
#include "memory.hpp"
#include "helpers.hpp"
#include "test_helpers.hpp"

using namespace M68K;

TEST_CASE("Exception: default state is supervisor", "[exception]") {
    CPUState state = CPUState();
    CHECK(state.registers.get(REG_SR, SIZE_WORD) == 0x2700);
    CHECK(state.registers.get(SR_FLAG_SUPERVISOR) == true);
}

TEST_CASE("Exception: A7 switches between USP and SSP", "[exception]") {
    CPUState state = CPUState();
    state.registers.set_usp(0x2000);
    state.registers.set_ssp(0x1000);
    CHECK(state.registers.get(REG_A7, SIZE_LONG) == 0x1000);

    state.registers.set(REG_SR, SIZE_WORD, 0x0000);
    CHECK(state.registers.get(REG_A7, SIZE_LONG) == 0x2000);

    state.registers.set(REG_A7, SIZE_LONG, 0x2100);
    CHECK(state.registers.get_usp() == 0x2100);
    CHECK(state.registers.get_ssp() == 0x1000);
}

TEST_CASE("Exception: SR keeps only implemented bits", "[exception]") {
    CPUState state = CPUState();
    state.registers.set(REG_SR, SIZE_WORD, 0xFFFF);
    CHECK(state.registers.get(REG_SR, SIZE_WORD) == 0xA71F);
}

TEST_CASE("Exception: frame from user mode", "[exception]") {
    CPUState state = CPUState();
    state.registers.set_ssp(0x1000);
    state.registers.set_usp(0x2000);
    state.registers.set(REG_SR, SIZE_WORD, 0x8005);
    state.registers.set(REG_PC, SIZE_LONG, 0x500);
    state.memory.set(VECTOR_TRAP * 4, SIZE_LONG, 0x3000);

    state.exception(VECTOR_TRAP);
    CHECK(state.registers.get(REG_PC, SIZE_LONG) == 0x3000);
    CHECK(state.registers.get(REG_SR, SIZE_WORD) == 0x2005);
    CHECK(state.registers.get(REG_A7, SIZE_LONG) == 0x0FFA);
    CHECK(state.registers.get_usp() == 0x2000);
    CHECK(state.memory.get(0x0FFA, SIZE_WORD) == 0x8005);
    CHECK(state.memory.get(0x0FFC, SIZE_LONG) == 0x500);
}

TEST_CASE("Exception: frame from supervisor mode", "[exception]") {
    CPUState state = CPUState();
    state.registers.set(REG_A7, SIZE_LONG, 0x1000);
    state.registers.set(REG_PC, SIZE_LONG, 0x500);
    state.memory.set(VECTOR_TRAP * 4, SIZE_LONG, 0x3000);
    state.stopped = true;

    state.exception(VECTOR_TRAP);
    CHECK(state.registers.get(REG_PC, SIZE_LONG) == 0x3000);
    CHECK(state.registers.get(REG_SR, SIZE_WORD) == 0x2700);
    CHECK(state.registers.get(REG_A7, SIZE_LONG) == 0x0FFA);
    CHECK(state.memory.get(0x0FFA, SIZE_WORD) == 0x2700);
    CHECK(state.memory.get(0x0FFC, SIZE_LONG) == 0x500);
    CHECK(state.stopped == false);
}

TEST_CASE("Exception: require_supervisor in user mode", "[exception]") {
    CPUState state = CPUState();
    state.registers.set_ssp(0x1000);
    state.registers.set(REG_SR, SIZE_WORD, 0x0000);
    state.registers.set(REG_PC, SIZE_LONG, 0x500);
    state.memory.set(VECTOR_PRIVILEGE_VIOLATION * 4, SIZE_LONG, 0x3000);

    CHECK(state.require_supervisor() == false);
    CHECK(state.registers.get(REG_PC, SIZE_LONG) == 0x3000);
    CHECK(state.memory.get(0x0FFC, SIZE_LONG) == 0x500);
    CHECK(state.require_supervisor() == true);
}

TEST_CASE("Exception: CPU reset", "[exception]") {
    CPU cpu = CPU();
    cpu.state.memory.set(0x0, SIZE_LONG, 0x1000);
    cpu.state.memory.set(0x4, SIZE_LONG, 0x400);
    cpu.state.registers.set(REG_SR, SIZE_WORD, 0x001F);
    cpu.state.stopped = true;
    cpu.state.halted = true;
    cpu.state.interrupt_level = 3;

    cpu.reset();
    CHECK(cpu.state.registers.get(REG_SR, SIZE_WORD) == 0x2700);
    CHECK(cpu.state.registers.get_ssp() == 0x1000);
    CHECK(cpu.state.registers.get(REG_A7, SIZE_LONG) == 0x1000);
    CHECK(cpu.state.registers.get(REG_PC, SIZE_LONG) == 0x400);
    CHECK(cpu.state.stopped == false);
    CHECK(cpu.state.halted == false);
    CHECK(cpu.state.interrupt_level == 0);
}

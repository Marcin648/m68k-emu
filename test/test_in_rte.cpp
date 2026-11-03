#include <catch2/catch_test_macros.hpp>

#include "m68k.hpp"
#include "memory.hpp"
#include "helpers.hpp"
#include "test_helpers.hpp"

using namespace M68K;

TEST_CASE("RTE: return to user mode", "[instruction][rte]") {
    auto instruction = INSTRUCTION::Rte::create(0x4E73);
    CPUState state = CPUState();
    state.registers.set_ssp(0x1000);
    state.registers.set_usp(0x3000);
    state.stack_push(SIZE_LONG, 0x2000);
    state.stack_push(SIZE_WORD, 0x0015);

    instruction.get()->execute(state);
    CHECK(state.registers.get(REG_PC, SIZE_LONG) == 0x2000);
    CHECK(state.registers.get(REG_SR, SIZE_WORD) == 0x0015);
    CHECK(state.registers.get_ssp() == 0x1000);
    CHECK(state.registers.get(REG_A7, SIZE_LONG) == 0x3000);
}

TEST_CASE("RTE: return to supervisor mode", "[instruction][rte]") {
    auto instruction = INSTRUCTION::Rte::create(0x4E73);
    CPUState state = CPUState();
    state.registers.set_ssp(0x1000);
    state.stack_push(SIZE_LONG, 0x2000);
    state.stack_push(SIZE_WORD, 0x2204);

    instruction.get()->execute(state);
    CHECK(state.registers.get(REG_PC, SIZE_LONG) == 0x2000);
    CHECK(state.registers.get(REG_SR, SIZE_WORD) == 0x2204);
    CHECK(state.registers.get(REG_A7, SIZE_LONG) == 0x1000);
}

TEST_CASE("RTE: privilege violation in user mode", "[instruction][rte]") {
    auto instruction = INSTRUCTION::Rte::create(0x4E73);
    CPUState state = CPUState();
    state.registers.set_ssp(0x1000);
    state.registers.set_usp(0x3000);
    state.registers.set(REG_SR, SIZE_WORD, 0x0000);
    state.registers.set(REG_PC, SIZE_LONG, 0x600);
    state.memory.set(VECTOR_PRIVILEGE_VIOLATION * 4, SIZE_LONG, 0x4000);

    instruction.get()->execute(state);
    CHECK(state.registers.get(REG_PC, SIZE_LONG) == 0x4000);
    CHECK(state.registers.get(REG_SR, SIZE_WORD) == 0x2000);
    CHECK(state.registers.get_ssp() == 0x0FFA);
    CHECK(state.registers.get_usp() == 0x3000);
    CHECK(state.memory.get(0x0FFC, SIZE_LONG) == 0x600);
}

TEST_CASE("RTE: trap and return", "[instruction][rte]") {
    CPU cpu = CPU();
    cpu.state.registers.set_ssp(0x1000);
    cpu.state.registers.set_usp(0x2000);
    cpu.state.registers.set(REG_SR, SIZE_WORD, 0x0000);
    cpu.state.registers.set(REG_PC, SIZE_LONG, 0x600);
    cpu.state.memory.set(0x600, SIZE_WORD, 0x4E40); // trap #0
    cpu.state.memory.set(0x3000, SIZE_WORD, 0x4E73); // rte
    cpu.state.memory.set(VECTOR_TRAP * 4, SIZE_LONG, 0x3000);

    cpu.step();
    CHECK(cpu.state.registers.get(REG_PC, SIZE_LONG) == 0x3000);
    cpu.step();
    CHECK(cpu.state.registers.get(REG_PC, SIZE_LONG) == 0x602);
    CHECK(cpu.state.registers.get(REG_SR, SIZE_WORD) == 0x0000);
    CHECK(cpu.state.registers.get_ssp() == 0x1000);
    CHECK(cpu.state.registers.get(REG_A7, SIZE_LONG) == 0x2000);
}

TEST_CASE("RTE: disassembly", "[instruction][rte]") {
    CPUState state = CPUState();
    CHECK(INSTRUCTION::Rte::create(0x4E73).get()->disassembly(state) == "rte");
}

#include <catch2/catch_test_macros.hpp>

#include "m68k.hpp"
#include "memory.hpp"
#include "helpers.hpp"
#include "test_helpers.hpp"

using namespace M68K;

TEST_CASE("TRAP: trap #5", "[instruction][trap]") {
    auto instruction = INSTRUCTION::Trap::create(0x4E45);
    CPUState state = CPUState();
    state.registers.set(REG_A7, SIZE_LONG, 0x1000);
    state.registers.set(REG_PC, SIZE_LONG, 0x600);
    state.memory.set((VECTOR_TRAP + 5) * 4, SIZE_LONG, 0x3000);

    instruction.get()->execute(state);
    CHECK(state.registers.get(REG_PC, SIZE_LONG) == 0x3000);
    CHECK(state.registers.get(REG_A7, SIZE_LONG) == 0x0FFA);
    CHECK(state.memory.get(0x0FFA, SIZE_WORD) == 0x2700);
    CHECK(state.memory.get(0x0FFC, SIZE_LONG) == 0x602);
}

TEST_CASE("TRAP: trap #15 from user mode", "[instruction][trap]") {
    CPU cpu = CPU();
    cpu.state.registers.set_ssp(0x1000);
    cpu.state.registers.set_usp(0x2000);
    cpu.state.registers.set(REG_SR, SIZE_WORD, 0x0004);
    cpu.state.registers.set(REG_PC, SIZE_LONG, 0x600);
    cpu.state.memory.set(0x600, SIZE_WORD, 0x4E4F);
    cpu.state.memory.set((VECTOR_TRAP + 15) * 4, SIZE_LONG, 0x3000);

    cpu.step();
    CHECK(cpu.state.registers.get(REG_PC, SIZE_LONG) == 0x3000);
    CHECK(cpu.state.registers.get(REG_SR, SIZE_WORD) == 0x2004);
    CHECK(cpu.state.registers.get_usp() == 0x2000);
    CHECK(cpu.state.registers.get_ssp() == 0x0FFA);
    CHECK(cpu.state.memory.get(0x0FFA, SIZE_WORD) == 0x0004);
    CHECK(cpu.state.memory.get(0x0FFC, SIZE_LONG) == 0x602);
}

TEST_CASE("TRAP: disassembly", "[instruction][trap]") {
    CPUState state = CPUState();
    CHECK(INSTRUCTION::Trap::create(0x4E4F).get()->disassembly(state) == "trap #$f");
}

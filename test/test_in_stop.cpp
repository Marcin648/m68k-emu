#include <catch2/catch_test_macros.hpp>

#include "m68k.hpp"
#include "memory.hpp"
#include "helpers.hpp"
#include "test_helpers.hpp"

using namespace M68K;

static void setup_cpu(CPU& cpu, uint16_t sr) {
    cpu.state.registers.set(REG_SR, SIZE_WORD, sr);
    cpu.state.registers.set(REG_A7, SIZE_LONG, 0x1000);
    cpu.state.registers.set(REG_PC, SIZE_LONG, 0x500);
    cpu.state.memory.set(0x500, SIZE_WORD, 0x4E72); // stop
    cpu.state.memory.set(0x502, SIZE_WORD, sr);
    cpu.state.memory.set(VECTOR_TRACE * 4, SIZE_LONG, 0x3000);
    cpu.state.memory.set((VECTOR_AUTOVECTOR + 1) * 4, SIZE_LONG, 0x3100);
}

TEST_CASE("STOP: stop #$2000", "[instruction][stop]") {
    CPU cpu = CPU();
    setup_cpu(cpu, 0x2000);

    cpu.step();
    CHECK(cpu.state.stopped == true);
    CHECK(cpu.state.registers.get(REG_SR, SIZE_WORD) == 0x2000);
    CHECK(cpu.state.registers.get(REG_PC, SIZE_LONG) == 0x504);

    cpu.step();
    CHECK(cpu.state.registers.get(REG_PC, SIZE_LONG) == 0x504);
}

TEST_CASE("STOP: resumed by interrupt", "[instruction][stop]") {
    CPU cpu = CPU();
    setup_cpu(cpu, 0x2000);

    cpu.step();
    cpu.interrupt(1);
    cpu.step();
    CHECK(cpu.state.stopped == false);
    CHECK(cpu.state.registers.get(REG_PC, SIZE_LONG) == 0x3100);
    CHECK(cpu.state.memory.get(0x0FFC, SIZE_LONG) == 0x504);
}

TEST_CASE("STOP: resumed by trace", "[instruction][stop]") {
    CPU cpu = CPU();
    setup_cpu(cpu, 0xA700);

    cpu.step();
    CHECK(cpu.state.stopped == false);
    CHECK(cpu.state.registers.get(REG_PC, SIZE_LONG) == 0x3000);
    CHECK(cpu.state.memory.get(0x0FFC, SIZE_LONG) == 0x504);
}

TEST_CASE("STOP: privilege violation", "[instruction][stop]") {
    check_privilege_violation(0x4E72);
}

TEST_CASE("STOP: disassembly", "[instruction][stop]") {
    CPUState state = CPUState();
    state.memory.set(2, SIZE_WORD, 0x2700);
    CHECK(INSTRUCTION::Stop::create(0x4E72).get()->disassembly(state) == "stop #$2700");
}

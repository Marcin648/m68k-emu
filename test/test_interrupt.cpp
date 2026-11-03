#include <catch2/catch_test_macros.hpp>

#include "m68k.hpp"
#include "memory.hpp"
#include "helpers.hpp"
#include "test_helpers.hpp"

using namespace M68K;

static void setup_cpu(CPU& cpu, uint16_t sr) {
    cpu.state.registers.set_ssp(0x1000);
    cpu.state.registers.set_usp(0x2000);
    cpu.state.registers.set(REG_SR, SIZE_WORD, sr);
    cpu.state.registers.set(REG_PC, SIZE_LONG, 0x500);
    cpu.state.memory.set(0x500, SIZE_WORD, 0x4E71); // nop
    cpu.state.memory.set((VECTOR_AUTOVECTOR + 3) * 4, SIZE_LONG, 0x3000);
    cpu.state.memory.set((VECTOR_AUTOVECTOR + 7) * 4, SIZE_LONG, 0x3700);
}

TEST_CASE("Interrupt: taken above mask", "[exception][interrupt]") {
    CPU cpu = CPU();
    setup_cpu(cpu, 0x2200);

    cpu.interrupt(3);
    cpu.step();
    CHECK(cpu.state.registers.get(REG_PC, SIZE_LONG) == 0x3000);
    CHECK(cpu.state.registers.get(REG_SR, SIZE_WORD) == 0x2300);
    CHECK(cpu.state.registers.get(REG_A7, SIZE_LONG) == 0x0FFA);
    CHECK(cpu.state.memory.get(0x0FFA, SIZE_WORD) == 0x2200);
    CHECK(cpu.state.memory.get(0x0FFC, SIZE_LONG) == 0x500);
    CHECK(cpu.state.interrupt_level == 0);
}

TEST_CASE("Interrupt: masked stays pending", "[exception][interrupt]") {
    CPU cpu = CPU();
    setup_cpu(cpu, 0x2300);

    cpu.interrupt(3);
    cpu.step();
    CHECK(cpu.state.registers.get(REG_PC, SIZE_LONG) == 0x502);
    CHECK(cpu.state.interrupt_level == 3);

    cpu.state.registers.set(REG_SR, SIZE_WORD, 0x2200);
    cpu.step();
    CHECK(cpu.state.registers.get(REG_PC, SIZE_LONG) == 0x3000);
}

TEST_CASE("Interrupt: level 7 is non-maskable", "[exception][interrupt]") {
    CPU cpu = CPU();
    setup_cpu(cpu, 0x2700);

    cpu.interrupt(7);
    cpu.step();
    CHECK(cpu.state.registers.get(REG_PC, SIZE_LONG) == 0x3700);
    CHECK(cpu.state.registers.get(REG_SR, SIZE_WORD) == 0x2700);
}

TEST_CASE("Interrupt: from user mode", "[exception][interrupt]") {
    CPU cpu = CPU();
    setup_cpu(cpu, 0x0004);

    cpu.interrupt(3);
    cpu.step();
    CHECK(cpu.state.registers.get(REG_PC, SIZE_LONG) == 0x3000);
    CHECK(cpu.state.registers.get(REG_SR, SIZE_WORD) == 0x2304);
    CHECK(cpu.state.registers.get_usp() == 0x2000);
    CHECK(cpu.state.registers.get_ssp() == 0x0FFA);
    CHECK(cpu.state.memory.get(0x0FFA, SIZE_WORD) == 0x0004);
}

TEST_CASE("Interrupt: wakes stopped CPU", "[exception][interrupt]") {
    CPU cpu = CPU();
    setup_cpu(cpu, 0x2000);
    cpu.state.stopped = true;

    cpu.step();
    CHECK(cpu.state.registers.get(REG_PC, SIZE_LONG) == 0x500);

    cpu.interrupt(3);
    cpu.step();
    CHECK(cpu.state.stopped == false);
    CHECK(cpu.state.registers.get(REG_PC, SIZE_LONG) == 0x3000);
}

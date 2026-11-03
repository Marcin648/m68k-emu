#include <catch2/catch_test_macros.hpp>

#include "m68k.hpp"
#include "memory.hpp"
#include "helpers.hpp"
#include "test_helpers.hpp"

using namespace M68K;

static void setup_cpu(CPU& cpu, uint16_t opcode) {
    cpu.state.registers.set(REG_SR, SIZE_WORD, 0xA700);
    cpu.state.registers.set(REG_A7, SIZE_LONG, 0x1000);
    cpu.state.registers.set(REG_PC, SIZE_LONG, 0x500);
    cpu.state.memory.set(0x500, SIZE_WORD, opcode);
    cpu.state.memory.set(VECTOR_TRACE * 4, SIZE_LONG, 0x3000);
    cpu.state.memory.set(VECTOR_ILLEGAL * 4, SIZE_LONG, 0x3400);
}

TEST_CASE("Trace: exception after instruction", "[exception][trace]") {
    CPU cpu = CPU();
    setup_cpu(cpu, 0x4E71); // nop

    cpu.step();
    CHECK(cpu.state.registers.get(REG_PC, SIZE_LONG) == 0x3000);
    CHECK(cpu.state.registers.get(REG_SR, SIZE_WORD) == 0x2700);
    CHECK(cpu.state.registers.get(REG_A7, SIZE_LONG) == 0x0FFA);
    CHECK(cpu.state.memory.get(0x0FFA, SIZE_WORD) == 0xA700);
    CHECK(cpu.state.memory.get(0x0FFC, SIZE_LONG) == 0x502);
}

TEST_CASE("Trace: not taken after illegal", "[exception][trace]") {
    CPU cpu = CPU();
    setup_cpu(cpu, 0x4AFC); // illegal

    cpu.step();
    CHECK(cpu.state.registers.get(REG_PC, SIZE_LONG) == 0x3400);
    CHECK(cpu.state.registers.get(REG_A7, SIZE_LONG) == 0x0FFA);
}

TEST_CASE("Trace: disabled", "[exception][trace]") {
    CPU cpu = CPU();
    setup_cpu(cpu, 0x4E71); // nop
    cpu.state.registers.set(REG_SR, SIZE_WORD, 0x2700);

    cpu.step();
    CHECK(cpu.state.registers.get(REG_PC, SIZE_LONG) == 0x502);
}

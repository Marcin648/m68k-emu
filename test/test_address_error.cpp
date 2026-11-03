#include <catch2/catch_test_macros.hpp>

#include "m68k.hpp"
#include "memory.hpp"
#include "helpers.hpp"
#include "test_helpers.hpp"

using namespace M68K;

static void setup_cpu(CPU& cpu, uint16_t opcode) {
    cpu.state.registers.set(REG_A7, SIZE_LONG, 0x1000);
    cpu.state.registers.set(REG_PC, SIZE_LONG, 0x400);
    cpu.state.memory.set(0x400, SIZE_WORD, opcode);
    cpu.state.memory.set(VECTOR_ADDRESS_ERROR * 4, SIZE_LONG, 0x4000);
}

TEST_CASE("Address error: odd read", "[exception][address_error]") {
    CPU cpu = CPU();
    setup_cpu(cpu, 0x3010); // move.w (A0), D0
    cpu.state.registers.set(REG_A0, SIZE_LONG, 0x2001);

    cpu.step();
    CHECK(cpu.state.registers.get(REG_PC, SIZE_LONG) == 0x4000);
    CHECK(cpu.state.registers.get(REG_SR, SIZE_WORD) == 0x2704);
    CHECK(cpu.state.registers.get(REG_A7, SIZE_LONG) == 0x0FF2);
    CHECK(cpu.state.memory.get(0x0FF2, SIZE_WORD) == 0x001D);
    CHECK(cpu.state.memory.get(0x0FF4, SIZE_LONG) == 0x2001);
    CHECK(cpu.state.memory.get(0x0FF8, SIZE_WORD) == 0x3010);
    CHECK(cpu.state.memory.get(0x0FFA, SIZE_WORD) == 0x2704);
    CHECK(cpu.state.memory.get(0x0FFC, SIZE_LONG) == 0x402);
}

TEST_CASE("Address error: odd write from user mode", "[exception][address_error]") {
    CPU cpu = CPU();
    setup_cpu(cpu, 0x3080); // move.w D0, (A0)
    cpu.state.registers.set_usp(0x2000);
    cpu.state.registers.set(REG_SR, SIZE_WORD, 0x0000);
    cpu.state.registers.set(REG_A0, SIZE_LONG, 0x2001);
    cpu.state.registers.set(REG_D0, SIZE_LONG, 0x1234);

    cpu.step();
    CHECK(cpu.state.registers.get(REG_PC, SIZE_LONG) == 0x4000);
    CHECK(cpu.state.registers.get(REG_SR, SIZE_WORD) == 0x2000);
    CHECK(cpu.state.registers.get_usp() == 0x2000);
    CHECK(cpu.state.registers.get_ssp() == 0x0FF2);
    CHECK(cpu.state.memory.get(0x0FF2, SIZE_WORD) == 0x0009);
    CHECK(cpu.state.memory.get(0x0FF4, SIZE_LONG) == 0x2001);
    CHECK(cpu.state.memory.get(0x0FFA, SIZE_WORD) == 0x0000);
}

TEST_CASE("Address error: odd PC", "[exception][address_error]") {
    CPU cpu = CPU();
    setup_cpu(cpu, 0x4E71);
    cpu.state.registers.set(REG_PC, SIZE_LONG, 0x401);

    cpu.step();
    CHECK(cpu.state.registers.get(REG_PC, SIZE_LONG) == 0x4000);
    CHECK(cpu.state.memory.get(0x0FF2, SIZE_WORD) == 0x0016);
    CHECK(cpu.state.memory.get(0x0FF4, SIZE_LONG) == 0x401);
    CHECK(cpu.state.memory.get(0x0FFC, SIZE_LONG) == 0x401);
}

TEST_CASE("Address error: double fault halts", "[exception][address_error]") {
    CPU cpu = CPU();
    setup_cpu(cpu, 0x3010); // move.w (A0), D0
    cpu.state.registers.set(REG_A7, SIZE_LONG, 0x1001);
    cpu.state.registers.set(REG_A0, SIZE_LONG, 0x2001);

    cpu.step();
    CHECK(cpu.state.halted == true);
    CHECK(cpu.state.memory.address_error.pending == false);

    uint32_t pc = cpu.state.registers.get(REG_PC, SIZE_LONG);
    cpu.step();
    CHECK(cpu.state.registers.get(REG_PC, SIZE_LONG) == pc);
}

TEST_CASE("Address error: interrupt with odd SSP halts", "[exception][address_error]") {
    CPU cpu = CPU();
    setup_cpu(cpu, 0x4E71); // nop
    cpu.state.registers.set(REG_A7, SIZE_LONG, 0x1001);

    cpu.interrupt(7);
    cpu.step();
    CHECK(cpu.state.halted == true);
}

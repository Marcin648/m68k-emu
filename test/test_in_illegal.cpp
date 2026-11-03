#include <catch2/catch_test_macros.hpp>

#include "m68k.hpp"
#include "memory.hpp"
#include "helpers.hpp"
#include "test_helpers.hpp"

using namespace M68K;

static void run_illegal(uint16_t opcode, uint8_t vector) {
    CPU cpu = CPU();
    cpu.state.registers.set(REG_A7, SIZE_LONG, 0x1000);
    cpu.state.registers.set(REG_PC, SIZE_LONG, 0x600);
    cpu.state.memory.set(0x600, SIZE_WORD, opcode);
    cpu.state.memory.set(vector * 4, SIZE_LONG, 0x3000);

    cpu.step();
    CHECK(cpu.state.registers.get(REG_PC, SIZE_LONG) == 0x3000);
    CHECK(cpu.state.registers.get(REG_A7, SIZE_LONG) == 0x0FFA);
    CHECK(cpu.state.memory.get(0x0FFA, SIZE_WORD) == 0x2700);
    CHECK(cpu.state.memory.get(0x0FFC, SIZE_LONG) == 0x600);
}

TEST_CASE("ILLEGAL: illegal", "[instruction][illegal]") {
    run_illegal(0x4AFC, VECTOR_ILLEGAL);
}

TEST_CASE("ILLEGAL: unknown opcode", "[instruction][illegal]") {
    run_illegal(0x4E7A, VECTOR_ILLEGAL);
}

TEST_CASE("ILLEGAL: line A", "[instruction][illegal]") {
    run_illegal(0xA123, VECTOR_LINE_A);
}

TEST_CASE("ILLEGAL: line F", "[instruction][illegal]") {
    run_illegal(0xF123, VECTOR_LINE_F);
}

TEST_CASE("ILLEGAL: disassembly", "[instruction][illegal]") {
    CPUState state = CPUState();
    CHECK(INSTRUCTION::Illegal::create(0x4AFC).get()->disassembly(state) == "illegal");
}

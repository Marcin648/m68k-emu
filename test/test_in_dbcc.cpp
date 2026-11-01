#include <catch2/catch_test_macros.hpp>

#include "m68k.hpp"
#include "memory.hpp"
#include "helpers.hpp"
#include "test_helpers.hpp"

using namespace M68K;

TEST_CASE("DBcc: dbf D0 (branch)", "[instruction][dbcc]") {
    auto instruction = INSTRUCTION::Dbcc::create(0x51C8);
    CPUState state = CPUState();
    state.registers.set(REG_PC, SIZE_LONG, 0x1000);
    state.memory.set(0x1000, SIZE_WORD, 0x51C8);
    state.memory.set(0x1002, SIZE_WORD, 0xFFFC);
    state.registers.set(REG_D0, SIZE_LONG, 0x00000002);

    instruction.get()->execute(state);
    CHECK(state.registers.get(REG_D0, SIZE_LONG) == 0x00000001);
    CHECK(state.registers.get(REG_PC, SIZE_LONG) == 0x0FFE);
}

TEST_CASE("DBcc: dbf D0 (counter expired)", "[instruction][dbcc]") {
    auto instruction = INSTRUCTION::Dbcc::create(0x51C8);
    CPUState state = CPUState();
    state.registers.set(REG_PC, SIZE_LONG, 0x1000);
    state.memory.set(0x1000, SIZE_WORD, 0x51C8);
    state.memory.set(0x1002, SIZE_WORD, 0xFFFC);
    state.registers.set(REG_D0, SIZE_LONG, 0xAAAA0000);

    instruction.get()->execute(state);
    CHECK(state.registers.get(REG_D0, SIZE_LONG) == 0xAAAAFFFF);
    CHECK(state.registers.get(REG_PC, SIZE_LONG) == 0x1004);
}

TEST_CASE("DBcc: dbeq D0 (condition true)", "[instruction][dbcc]") {
    auto instruction = INSTRUCTION::Dbcc::create(0x57C8);
    CPUState state = CPUState();
    state.registers.set(REG_PC, SIZE_LONG, 0x1000);
    state.memory.set(0x1000, SIZE_WORD, 0x57C8);
    state.memory.set(0x1002, SIZE_WORD, 0xFFFC);
    state.registers.set(REG_D0, SIZE_LONG, 0x00000005);
    state.registers.set(SR_FLAG_ZERO, true);

    instruction.get()->execute(state);
    CHECK(state.registers.get(REG_D0, SIZE_LONG) == 0x00000005);
    CHECK(state.registers.get(REG_PC, SIZE_LONG) == 0x1004);
}

TEST_CASE("DBcc: dbne D0 (condition false)", "[instruction][dbcc]") {
    auto instruction = INSTRUCTION::Dbcc::create(0x56C8);
    CPUState state = CPUState();
    state.registers.set(REG_PC, SIZE_LONG, 0x1000);
    state.memory.set(0x1000, SIZE_WORD, 0x56C8);
    state.memory.set(0x1002, SIZE_WORD, 0x0010);
    state.registers.set(REG_D0, SIZE_LONG, 0x00000005);
    state.registers.set(SR_FLAG_ZERO, true);

    instruction.get()->execute(state);
    CHECK(state.registers.get(REG_D0, SIZE_LONG) == 0x00000004);
    CHECK(state.registers.get(REG_PC, SIZE_LONG) == 0x1012);
}

TEST_CASE("DBcc: disassembly", "[instruction][dbcc]") {
    CPUState state = CPUState();
    state.registers.set(REG_PC, SIZE_LONG, 0x1000);
    state.memory.set(0x1002, SIZE_WORD, 0xFFFC);
    CHECK(INSTRUCTION::Dbcc::create(0x51C8).get()->disassembly(state) == "dbf d0, $ffe");
}

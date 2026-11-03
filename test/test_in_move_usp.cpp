#include <catch2/catch_test_macros.hpp>

#include "m68k.hpp"
#include "memory.hpp"
#include "helpers.hpp"
#include "test_helpers.hpp"

using namespace M68K;

TEST_CASE("MOVE USP: move.l A0, usp", "[instruction][move_usp]") {
    auto instruction = INSTRUCTION::MoveUsp::create(0x4E60);
    CPUState state = CPUState();
    state.registers.set(REG_A7, SIZE_LONG, 0x1000);
    state.registers.set(REG_A0, SIZE_LONG, 0x3000);

    instruction.get()->execute(state);
    CHECK(state.registers.get_usp() == 0x3000);
    CHECK(state.registers.get(REG_A7, SIZE_LONG) == 0x1000);
    CHECK(state.registers.get(REG_PC, SIZE_LONG) == 2);
}

TEST_CASE("MOVE USP: move.l usp, A1", "[instruction][move_usp]") {
    auto instruction = INSTRUCTION::MoveUsp::create(0x4E69);
    CPUState state = CPUState();
    state.registers.set_usp(0x5000);

    instruction.get()->execute(state);
    CHECK(state.registers.get(REG_A1, SIZE_LONG) == 0x5000);
    CHECK(state.registers.get(REG_PC, SIZE_LONG) == 2);
}

TEST_CASE("MOVE USP: privilege violation", "[instruction][move_usp]") {
    check_privilege_violation(0x4E60);
    check_privilege_violation(0x4E69);
}

TEST_CASE("MOVE USP: disassembly", "[instruction][move_usp]") {
    CPUState state = CPUState();
    CHECK(INSTRUCTION::MoveUsp::create(0x4E60).get()->disassembly(state) == "move.l a0, usp");
    CHECK(INSTRUCTION::MoveUsp::create(0x4E69).get()->disassembly(state) == "move.l usp, a1");
}

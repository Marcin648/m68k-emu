#include <catch2/catch_test_macros.hpp>

#include "m68k.hpp"
#include "memory.hpp"
#include "helpers.hpp"
#include "test_helpers.hpp"

using namespace M68K;

TEST_CASE("RESET: reset", "[instruction][reset]") {
    auto instruction = INSTRUCTION::Reset::create(0x4E70);
    CPUState state = CPUState();
    state.registers.set(REG_PC, SIZE_LONG, 0x600);

    instruction.get()->execute(state);
    CHECK(state.registers.get(REG_PC, SIZE_LONG) == 0x602);
    CHECK(state.registers.get(REG_SR, SIZE_WORD) == 0x2700);
}

TEST_CASE("RESET: privilege violation", "[instruction][reset]") {
    check_privilege_violation(0x4E70);
}

TEST_CASE("RESET: disassembly", "[instruction][reset]") {
    CPUState state = CPUState();
    CHECK(INSTRUCTION::Reset::create(0x4E70).get()->disassembly(state) == "reset");
}

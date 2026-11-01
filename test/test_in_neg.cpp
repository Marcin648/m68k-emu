#include <catch2/catch_test_macros.hpp>

#include "m68k.hpp"
#include "memory.hpp"
#include "helpers.hpp"
#include "test_helpers.hpp"

using namespace M68K;

TEST_CASE("NEG: neg.l D0", "[instruction][neg]") {
    auto instruction = INSTRUCTION::Neg::create(0x4480);
    CPUState state = CPUState();
    state.registers.set(REG_D0, SIZE_LONG, 0x00000001);

    instruction.get()->execute(state);
    CHECK(state.registers.get(REG_D0, SIZE_LONG) == 0xFFFFFFFF);
    check_flags(state, true, true, false, false, true);
}

TEST_CASE("NEG: neg.b D0 (overflow)", "[instruction][neg]") {
    auto instruction = INSTRUCTION::Neg::create(0x4400);
    CPUState state = CPUState();
    state.registers.set(REG_D0, SIZE_LONG, 0x00000080);

    instruction.get()->execute(state);
    CHECK(state.registers.get(REG_D0, SIZE_LONG) == 0x00000080);
    check_flags(state, true, true, false, true, true);
}

TEST_CASE("NEG: neg.w D0 (zero)", "[instruction][neg]") {
    auto instruction = INSTRUCTION::Neg::create(0x4440);
    CPUState state = CPUState();
    state.registers.set(REG_D0, SIZE_LONG, 0x00000000);

    instruction.get()->execute(state);
    CHECK(state.registers.get(REG_D0, SIZE_LONG) == 0x00000000);
    check_flags(state, false, false, true, false, false);
}

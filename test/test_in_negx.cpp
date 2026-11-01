#include <catch2/catch_test_macros.hpp>

#include "m68k.hpp"
#include "memory.hpp"
#include "helpers.hpp"
#include "test_helpers.hpp"

using namespace M68K;

TEST_CASE("NEGX: negx.b D0", "[instruction][negx]") {
    auto instruction = INSTRUCTION::Negx::create(0x4000);
    CPUState state = CPUState();
    state.registers.set(REG_D0, SIZE_LONG, 0x00000001);
    state.registers.set(SR_FLAG_ZERO, true);

    instruction.get()->execute(state);
    CHECK(state.registers.get(REG_D0, SIZE_LONG) == 0x000000FF);
    check_flags(state, true, true, false, false, true);
}

TEST_CASE("NEGX: negx.b D0 (extend)", "[instruction][negx]") {
    auto instruction = INSTRUCTION::Negx::create(0x4000);
    CPUState state = CPUState();
    state.registers.set(REG_D0, SIZE_LONG, 0x00000000);
    state.registers.set(SR_FLAG_EXTEND, true);

    instruction.get()->execute(state);
    CHECK(state.registers.get(REG_D0, SIZE_LONG) == 0x000000FF);
    check_flags(state, true, true, false, false, true);
}

TEST_CASE("NEGX: negx.w D0 (zero keeps Z)", "[instruction][negx]") {
    auto instruction = INSTRUCTION::Negx::create(0x4040);
    CPUState state = CPUState();
    state.registers.set(REG_D0, SIZE_LONG, 0xAAAA0000);
    state.registers.set(SR_FLAG_ZERO, true);

    instruction.get()->execute(state);
    CHECK(state.registers.get(REG_D0, SIZE_LONG) == 0xAAAA0000);
    check_flags(state, false, false, true, false, false);
}

TEST_CASE("NEGX: negx.l D0 (extend, zero result)", "[instruction][negx]") {
    auto instruction = INSTRUCTION::Negx::create(0x4080);
    CPUState state = CPUState();
    state.registers.set(REG_D0, SIZE_LONG, 0xFFFFFFFF);
    state.registers.set(SR_FLAG_EXTEND, true);
    state.registers.set(SR_FLAG_ZERO, true);

    instruction.get()->execute(state);
    CHECK(state.registers.get(REG_D0, SIZE_LONG) == 0x00000000);
    check_flags(state, true, false, true, false, true);
}

TEST_CASE("NEGX: negx.b D0 (overflow)", "[instruction][negx]") {
    auto instruction = INSTRUCTION::Negx::create(0x4000);
    CPUState state = CPUState();
    state.registers.set(REG_D0, SIZE_LONG, 0x00000080);

    instruction.get()->execute(state);
    CHECK(state.registers.get(REG_D0, SIZE_LONG) == 0x00000080);
    check_flags(state, true, true, false, true, true);
}

TEST_CASE("NEGX: validity and disassembly", "[instruction][negx]") {
    CHECK(INSTRUCTION::Negx::create(0x40C0).get()->is_valid == false); // move from sr
    CHECK(INSTRUCTION::Negx::create(0x4048).get()->is_valid == false); // negx.w A0

    CPUState state = CPUState();
    CHECK(INSTRUCTION::Negx::create(0x4080).get()->disassembly(state) == "negx.l d0");
}

#include <catch2/catch_test_macros.hpp>

#include "m68k.hpp"
#include "memory.hpp"
#include "helpers.hpp"
#include "test_helpers.hpp"

using namespace M68K;

TEST_CASE("TAS: tas D0 (zero)", "[instruction][tas]") {
    auto instruction = INSTRUCTION::Tas::create(0x4AC0);
    CPUState state = CPUState();
    state.registers.set(REG_D0, SIZE_LONG, 0x12345600);

    instruction.get()->execute(state);
    CHECK(state.registers.get(REG_D0, SIZE_LONG) == 0x12345680);
    check_flags(state, false, false, true, false, false);
}

TEST_CASE("TAS: tas (A0) (negative)", "[instruction][tas]") {
    auto instruction = INSTRUCTION::Tas::create(0x4AD0);
    CPUState state = CPUState();
    state.registers.set(REG_A0, SIZE_LONG, 0x1000);
    state.memory.set(0x1000, SIZE_BYTE, 0x81);
    state.registers.set(SR_FLAG_OVERFLOW, true);
    state.registers.set(SR_FLAG_CARRY, true);

    instruction.get()->execute(state);
    CHECK(state.memory.get(0x1000, SIZE_BYTE) == 0x81);
    check_flags(state, false, true, false, false, false);
}

TEST_CASE("TAS: validity and disassembly", "[instruction][tas]") {
    CHECK(INSTRUCTION::Tas::create(0x4AC8).get()->is_valid == false); // tas A0
    CHECK(INSTRUCTION::Tas::create(0x4AFA).get()->is_valid == false); // tas d16(PC)

    CPUState state = CPUState();
    CHECK(INSTRUCTION::Tas::create(0x4AD0).get()->disassembly(state) == "tas (a0)");
}

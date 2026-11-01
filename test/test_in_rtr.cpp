#include <catch2/catch_test_macros.hpp>

#include "m68k.hpp"
#include "memory.hpp"
#include "helpers.hpp"
#include "test_helpers.hpp"

using namespace M68K;

TEST_CASE("RTR: rtr", "[instruction][rtr]") {
    auto instruction = INSTRUCTION::Rtr::create(0x4E77);
    CPUState state = CPUState();
    state.registers.set(REG_A7, SIZE_LONG, 0x1000);
    state.registers.set(REG_SR, SIZE_WORD, 0x2700);
    state.stack_push(SIZE_LONG, 0x00002000);
    state.stack_push(SIZE_WORD, 0xFFFF);

    instruction.get()->execute(state);
    CHECK(state.registers.get(REG_PC, SIZE_LONG) == 0x2000);
    CHECK(state.registers.get(REG_A7, SIZE_LONG) == 0x1000);
    CHECK(state.registers.get(REG_SR, SIZE_WORD) == 0x271F);
}

TEST_CASE("RTR: disassembly", "[instruction][rtr]") {
    CPUState state = CPUState();
    CHECK(INSTRUCTION::Rtr::create(0x4E77).get()->disassembly(state) == "rtr");
}

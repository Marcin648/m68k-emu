#include <catch2/catch_test_macros.hpp>

#include "m68k.hpp"
#include "memory.hpp"
#include "helpers.hpp"

using namespace M68K;

TEST_CASE("NOP: nop", "[instruction][nop]") {
    auto instruction = INSTRUCTION::Nop::create(0x4E71); // nop
    CPUState state = CPUState();


    instruction.get()->execute(state);
    uint32_t return_data = state.registers.get(REG_PC, DataSize::SIZE_LONG);
    CHECK(return_data == 0x00000002);
}

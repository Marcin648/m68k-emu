#include <catch2/catch_test_macros.hpp>

#include "m68k.hpp"
#include "memory.hpp"
#include "helpers.hpp"

using namespace M68K;

TEST_CASE("LEA: lea 0xAABBCCDD, A1", "[instruction][lea]") {
    auto instruction = INSTRUCTION::Lea::create(0x43F9); // lea 0xAABBCCDD, A1
    CPUState state = CPUState();

    state.memory.set(0x0000, SIZE_WORD, 0x43F9); // opcode
    state.memory.set(0x0002, SIZE_LONG, 0xAABBCCDD); // address

    instruction.get()->execute(state);
    uint32_t return_data = state.registers.get(REG_A1, DataSize::SIZE_LONG);
    CHECK(return_data == 0xAABBCCDD);
}

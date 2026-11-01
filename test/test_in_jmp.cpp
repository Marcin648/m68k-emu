#include <catch2/catch_test_macros.hpp>

#include "m68k.hpp"
#include "memory.hpp"
#include "helpers.hpp"

using namespace M68K;

TEST_CASE("JMP: jmp (A0)", "[instruction][jmp]") {
    auto instruction = INSTRUCTION::Jmp::create(0x4ED0); // jmp (A0)
    CPUState state = CPUState();
    
    state.registers.set(REG_A0, DataSize::SIZE_LONG, 0x2000);

    instruction.get()->execute(state);
    uint32_t return_data = state.registers.get(REG_PC, DataSize::SIZE_LONG);
    CHECK(return_data == 0x2000);
}

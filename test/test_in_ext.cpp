#include <catch2/catch_test_macros.hpp>

#include "m68k.hpp"
#include "memory.hpp"
#include "helpers.hpp"
#include "test_helpers.hpp"

using namespace M68K;

TEST_CASE("EXT: ext.w D1", "[instruction][ext]") {
    auto instruction = INSTRUCTION::Ext::create(0x4881); // ext.w D1
    CPUState state = CPUState();
    
    state.registers.set(REG_D1, SIZE_BYTE, static_cast<int8_t>(-10));
    instruction.get()->execute(state);

    int16_t return_data = state.registers.get(REG_D1, DataSize::SIZE_WORD);
    CHECK(return_data == -10);
}

TEST_CASE("EXT: ext.l D1", "[instruction][ext]") {
    auto instruction = INSTRUCTION::Ext::create(0x48c1); // ext.l D1
    CPUState state = CPUState();
    
    state.registers.set(REG_D1, SIZE_WORD, static_cast<int16_t>(-10));
    instruction.get()->execute(state);

    int32_t return_data = state.registers.get(REG_D1, DataSize::SIZE_LONG);
    CHECK(return_data == -10);
}

TEST_CASE("EXT: ext.l D1 (positive word)", "[instruction][ext]") {
    auto instruction = INSTRUCTION::Ext::create(0x48C1);
    CPUState state = CPUState();
    state.registers.set(REG_D1, SIZE_LONG, 0xAAAA0080);

    instruction.get()->execute(state);
    CHECK(state.registers.get(REG_D1, SIZE_LONG) == 0x00000080);
    checkFlags(state, false, false, false, false, false);
}

TEST_CASE("EXT: ext.l D1 (negative word)", "[instruction][ext]") {
    auto instruction = INSTRUCTION::Ext::create(0x48C1);
    CPUState state = CPUState();
    state.registers.set(REG_D1, SIZE_LONG, 0x00008000);

    instruction.get()->execute(state);
    CHECK(state.registers.get(REG_D1, SIZE_LONG) == 0xFFFF8000);
    checkFlags(state, false, true, false, false, false);
}

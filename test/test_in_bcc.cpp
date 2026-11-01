#include <catch2/catch_test_macros.hpp>

#include "m68k.hpp"
#include "memory.hpp"
#include "helpers.hpp"
#include "test_helpers.hpp"

using namespace M68K;

TEST_CASE("Bcc: bvs.s 0x04 (V set)", "[instruction][bcc]") {
    uint32_t return_data;
    CPUState state = CPUState();

    auto instruction = INSTRUCTION::Bcc::create(0x6902); // bvs.s 0x04

    state.registers.set(SR_FLAG_OVERFLOW, true);
    state.registers.set(REG_PC, SIZE_LONG, 0x1000);

    instruction.get()->execute(state);
    return_data = state.registers.get(REG_PC, DataSize::SIZE_LONG);
    CHECK(return_data == 0x1004);
}

TEST_CASE("Bcc: bvs.s 0x04 (V clear)", "[instruction][bcc]") {
    uint32_t return_data;
    CPUState state = CPUState();

    auto instruction = INSTRUCTION::Bcc::create(0x6902); // bvs.s 0x04

    state.registers.set(SR_FLAG_OVERFLOW, false);
    state.registers.set(REG_PC, SIZE_LONG, 0x1000);

    instruction.get()->execute(state);
    return_data = state.registers.get(REG_PC, DataSize::SIZE_LONG);
    CHECK(return_data == 0x1002);
}

TEST_CASE("Bcc: bvs.w 0x1002", "[instruction][bcc]") {
    uint32_t return_data;
    CPUState state = CPUState();

    auto instruction = INSTRUCTION::Bcc::create(0x6900); // bvs.w 0x1002

    state.registers.set(SR_FLAG_OVERFLOW, true);
    state.registers.set(REG_PC, SIZE_LONG, 0x1000);

    state.memory.set(0x1000, SIZE_WORD, 0x6900); // opcode
    state.memory.set(0x1002, SIZE_WORD, 0x1000); // data

    instruction.get()->execute(state);
    return_data = state.registers.get(REG_PC, DataSize::SIZE_LONG);
    CHECK(return_data == 0x2002);
}

TEST_CASE("Bcc: bvs.l 0x00010002 (V set)", "[instruction][bcc]") {
    uint32_t return_data;
    CPUState state = CPUState();

    auto instruction = INSTRUCTION::Bcc::create(0x69FF); // bvs.l 0x00010002

    state.registers.set(SR_FLAG_OVERFLOW, true);
    state.registers.set(REG_PC, SIZE_LONG, 0x1000);

    state.memory.set(0x1000, SIZE_WORD, 0x69FF); // opcode
    state.memory.set(0x1002, SIZE_LONG, 0x10000); // data

    instruction.get()->execute(state);
    return_data = state.registers.get(REG_PC, DataSize::SIZE_LONG);
    CHECK(return_data == 0x11002);
}

TEST_CASE("Bcc: bvs.l 0x00010002 (V clear)", "[instruction][bcc]") {
    uint32_t return_data;
    CPUState state = CPUState();

    auto instruction = INSTRUCTION::Bcc::create(0x69FF); // bvs.l 0x00010002

    state.registers.set(SR_FLAG_OVERFLOW, false);
    state.registers.set(REG_PC, SIZE_LONG, 0x1000);

    state.memory.set(0x1000, SIZE_WORD, 0x69FF); // opcode
    state.memory.set(0x1002, SIZE_LONG, 0x10000); // data

    instruction.get()->execute(state);
    return_data = state.registers.get(REG_PC, DataSize::SIZE_LONG);
    CHECK(return_data == 0x1006);
}

TEST_CASE("Bcc: bsr.w pushes address after extension word", "[instruction][bcc]") {
    auto instruction = INSTRUCTION::Bcc::create(0x6100); // bsr.w
    CPUState state = CPUState();

    state.registers.set(REG_USP, SIZE_LONG, 0x8000);
    state.registers.set(REG_PC, SIZE_LONG, 0x1000);
    state.memory.set(0x1000, SIZE_WORD, 0x6100); // opcode
    state.memory.set(0x1002, SIZE_WORD, 0x0010); // displacement

    instruction.get()->execute(state);
    CHECK(state.registers.get(REG_PC, SIZE_LONG) == 0x1012);
    CHECK(state.registers.get(REG_USP, SIZE_LONG) == 0x7FFC);
    CHECK(state.memory.get(0x7FFC, SIZE_LONG) == 0x1004);
}

TEST_CASE("Bcc: bsr.s pushes address after opcode", "[instruction][bcc]") {
    auto instruction = INSTRUCTION::Bcc::create(0x6110); // bsr.s
    CPUState state = CPUState();

    state.registers.set(REG_USP, SIZE_LONG, 0x8000);
    state.registers.set(REG_PC, SIZE_LONG, 0x1000);

    instruction.get()->execute(state);
    CHECK(state.registers.get(REG_PC, SIZE_LONG) == 0x1012);
    CHECK(state.memory.get(0x7FFC, SIZE_LONG) == 0x1002);
}

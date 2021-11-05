#include "tests_functions.hpp"

#include "m68k.hpp"
#include "memory.hpp"
#include "helpers.hpp"

int main(int, char**){
    TEST_NAME("Instruction MOVE");

    M68K::InstructionDecoder decoder = M68K::InstructionDecoder();
    M68K::INSTRUCTION::Instruction* instruction = nullptr;
    uint32_t return_data = 0;

    M68K::CPUState state = M68K::CPUState();
    
    // move.w D1, A1
    TEST_LABEL("move.w A1, D1");
    state = M68K::CPUState();
    state.registers.set(M68K::REG_A1, M68K::DataSize::SIZE_LONG, 0xAABBCCDD);
    instruction = decoder.Decode(0x3209); // move.w D1, A1
    instruction->execute(state);
    return_data = state.registers.get(M68K::REG_D1, M68K::DataSize::SIZE_LONG);
    TEST_TRUE(return_data == 0xCCDD);

    // move.l A0, D1
    TEST_LABEL("move.l A0, D1");
    state = M68K::CPUState();
    state.registers.set(M68K::REG_D1, M68K::DataSize::SIZE_LONG, 0xAABBCCDD);
    instruction = decoder.Decode(0x2041); // move.l A0, D1
    instruction->execute(state);
    return_data = state.registers.get(M68K::REG_A0, M68K::DataSize::SIZE_LONG);
    TEST_TRUE(return_data == 0xAABBCCDD);

    // Test status register
    TEST_LABEL("Status register");
    state = M68K::CPUState();
    state.registers.set(M68K::REG_D1, M68K::DataSize::SIZE_LONG, 0xAABBCCDD);
    instruction = decoder.Decode(0x2041); // move.l A0, D1
    instruction->execute(state);

    bool flag_negative = state.registers.get(M68K::SR_FLAG_NEGATIVE);
    bool flag_zero = state.registers.get(M68K::SR_FLAG_ZERO);
    bool flag_overflow = state.registers.get(M68K::SR_FLAG_OVERFLOW);
    bool flag_carry = state.registers.get(M68K::SR_FLAG_CARRY);

    TEST_TRUE(flag_negative == true);
    TEST_TRUE(flag_zero == false);
    TEST_TRUE(flag_overflow == 0);
    TEST_TRUE(flag_carry == 0);
}
#include "tests_functions.hpp"

#include "m68k.hpp"
#include "memory.hpp"
#include "helpers.hpp"

int main(int, char**){
    TEST_NAME("Instruction MOVE / MOVEA");
    
    {
        TEST_LABEL("direct - move.w A1, D1");
        auto instruction = M68K::INSTRUCTION::Move::create(0x3209); // move.w D1, A1
        M68K::CPUState state = M68K::CPUState();
        
        state.registers.set(M68K::REG_A1, M68K::DataSize::SIZE_LONG, 0xAABBCCDD);

        instruction.get()->execute(state);
        uint32_t return_data = state.registers.get(M68K::REG_D1, M68K::DataSize::SIZE_LONG);
        TEST_TRUE(return_data == 0xCCDD);
    }

    {
        TEST_LABEL("direct - movea.l A0, D1");
        auto instruction = M68K::INSTRUCTION::Move::create(0x2041); // movea.l A0, D1
        M68K::CPUState state = M68K::CPUState();
        
        state.registers.set(M68K::REG_D1, M68K::DataSize::SIZE_LONG, 0xAABBCCDD);

        instruction.get()->execute(state);
        uint32_t return_data = state.registers.get(M68K::REG_A0, M68K::DataSize::SIZE_LONG);
        TEST_TRUE(return_data == 0xAABBCCDD);
    }
    
    {
        TEST_LABEL("indirect - move.l (A0), D1");
        auto instruction = M68K::INSTRUCTION::Move::create(0x2081); // move.l (A0), D1
        M68K::CPUState state = M68K::CPUState();
        
        state.registers.set(M68K::REG_D1, M68K::DataSize::SIZE_LONG, 0xAABBCCDD);
        state.registers.set(M68K::REG_A0, M68K::DataSize::SIZE_LONG, 0x1000);

        instruction.get()->execute(state);
        uint32_t return_data = state.memory.get(0x1000, M68K::DataSize::SIZE_LONG);
        TEST_TRUE(return_data == 0xAABBCCDD);
    }

    {
        TEST_LABEL("indirect++ - move.l (A0)+, D1");
        auto instruction = M68K::INSTRUCTION::Move::create(0x20c1); // move.l (A0)+, D1
        M68K::CPUState state = M68K::CPUState();
        
        state.registers.set(M68K::REG_D1, M68K::DataSize::SIZE_LONG, 0xAABBCCDD);
        state.registers.set(M68K::REG_A0, M68K::DataSize::SIZE_LONG, 0x1000);

        instruction.get()->execute(state);
        uint32_t return_data = state.registers.get(M68K::REG_A0, M68K::DataSize::SIZE_LONG);
        TEST_TRUE(return_data == 0x1004);
    }

    {
        TEST_LABEL("--indirect - move.l -(A0), D1");
        auto instruction = M68K::INSTRUCTION::Move::create(0x2101); // move.l -(A0), D1
        M68K::CPUState state = M68K::CPUState();
        
        state.registers.set(M68K::REG_D1, M68K::DataSize::SIZE_LONG, 0xAABBCCDD);
        state.registers.set(M68K::REG_A0, M68K::DataSize::SIZE_LONG, 0x1000);

        instruction.get()->execute(state);
        uint32_t return_data = state.registers.get(M68K::REG_A0, M68K::DataSize::SIZE_LONG);
        TEST_TRUE(return_data == 0x0ffc);
    }

    {
        TEST_LABEL("Status register");
        auto instruction = M68K::INSTRUCTION::Move::create(0x3209); // move.w D1, A1
        M68K::CPUState state = M68K::CPUState();
        
        state.registers.set(M68K::REG_A1, M68K::DataSize::SIZE_LONG, 0xAABBCCDD);

        instruction.get()->execute(state);
        bool flag_negative = state.registers.get(M68K::SR_FLAG_NEGATIVE);
        bool flag_zero = state.registers.get(M68K::SR_FLAG_ZERO);
        bool flag_overflow = state.registers.get(M68K::SR_FLAG_OVERFLOW);
        bool flag_carry = state.registers.get(M68K::SR_FLAG_CARRY);

        TEST_TRUE(flag_negative == true);
        TEST_TRUE(flag_zero == false);
        TEST_TRUE(flag_overflow == 0);
        TEST_TRUE(flag_carry == 0);
    }
}
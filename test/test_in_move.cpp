#include "tests_functions.hpp"

#include "m68k.hpp"
#include "memory.hpp"
#include "helpers.hpp"

int main(int, char**){
    TEST_NAME("Instruction MOVE / MOVEA");
    
    {
        TEST_LABEL("direct - move.w A1, D1");
        auto instruction = M68K::INSTRUCTION::Move::create(0x3209); // move.w A1, D1
        M68K::CPUState state = M68K::CPUState();
        
        state.registers.set(M68K::REG_A1, M68K::DataSize::SIZE_LONG, 0xAABBCCDD);

        instruction.get()->execute(state);
        uint32_t return_data = state.registers.get(M68K::REG_D1, M68K::DataSize::SIZE_LONG);
        TEST_TRUE(return_data == 0xCCDD);
    }

    {
        TEST_LABEL("addr direct - movea.l D1, A0");
        auto instruction = M68K::INSTRUCTION::Move::create(0x2041); // movea.l D1, A0
        M68K::CPUState state = M68K::CPUState();
        
        state.registers.set(M68K::REG_D1, M68K::DataSize::SIZE_LONG, 0xAABBCCDD);

        instruction.get()->execute(state);
        uint32_t return_data = state.registers.get(M68K::REG_A0, M68K::DataSize::SIZE_LONG);
        TEST_TRUE(return_data == 0xAABBCCDD);
    }
    
    {
        TEST_LABEL("addr indirect - move.l D1, (A0)");
        auto instruction = M68K::INSTRUCTION::Move::create(0x2081); // move.l D1, (A0)
        M68K::CPUState state = M68K::CPUState();
        
        state.registers.set(M68K::REG_D1, M68K::DataSize::SIZE_LONG, 0xAABBCCDD);
        state.registers.set(M68K::REG_A0, M68K::DataSize::SIZE_LONG, 0x1000);

        instruction.get()->execute(state);
        uint32_t return_data = state.memory.get(0x1000, M68K::DataSize::SIZE_LONG);
        TEST_TRUE(return_data == 0xAABBCCDD);
    }

    {
        TEST_LABEL("addr indirect++ - move.l D1, (A0)+");
        auto instruction = M68K::INSTRUCTION::Move::create(0x20C1); // move.l D1, (A0)+
        M68K::CPUState state = M68K::CPUState();
        
        state.registers.set(M68K::REG_D1, M68K::DataSize::SIZE_LONG, 0xAABBCCDD);
        state.registers.set(M68K::REG_A0, M68K::DataSize::SIZE_LONG, 0x1000);

        instruction.get()->execute(state);
        uint32_t return_data = state.registers.get(M68K::REG_A0, M68K::DataSize::SIZE_LONG);
        TEST_TRUE(return_data == 0x1004);
    }

    {
        TEST_LABEL("addr --indirect - move.l D1, -(A0)");
        auto instruction = M68K::INSTRUCTION::Move::create(0x2101); // move.l D1, -(A0)
        M68K::CPUState state = M68K::CPUState();
        
        state.registers.set(M68K::REG_D1, M68K::DataSize::SIZE_LONG, 0xAABBCCDD);
        state.registers.set(M68K::REG_A0, M68K::DataSize::SIZE_LONG, 0x1000);

        instruction.get()->execute(state);
        uint32_t return_data = state.registers.get(M68K::REG_A0, M68K::DataSize::SIZE_LONG);
        TEST_TRUE(return_data == 0x0ffc);
    }

    {
        TEST_LABEL("addr displacement - move.w (A0, 4), (A1, 8)");
        auto instruction = M68K::INSTRUCTION::Move::create(0x3368); // move.w (A0, 4), (A1, 8)
        M68K::CPUState state = M68K::CPUState();
        
        state.memory.set(0, M68K::DataSize::SIZE_WORD, 0x3368); // opcode
        state.memory.set(2, M68K::DataSize::SIZE_WORD, 0x0004); // src offset
        state.memory.set(4, M68K::DataSize::SIZE_WORD, 0x0008); // dest offset

        state.registers.set(M68K::REG_A0, M68K::DataSize::SIZE_LONG, 0x1000);
        state.registers.set(M68K::REG_A1, M68K::DataSize::SIZE_LONG, 0x2000);

        state.memory.set(0x1004, M68K::DataSize::SIZE_WORD, 0xCCDD); // dest offset

        instruction.get()->execute(state);
        uint32_t return_data = state.memory.get(0x2008, M68K::DataSize::SIZE_WORD);
        TEST_TRUE(return_data == 0xCCDD);
    }

    {
        TEST_LABEL("addr displacement negative - move.w (A0, 4), (A1, -2)");
        auto instruction = M68K::INSTRUCTION::Move::create(0x3368); // move.w (A0, 4), (A1, -2)
        M68K::CPUState state = M68K::CPUState();
        
        state.memory.set(0, M68K::DataSize::SIZE_WORD, 0x3368); // opcode
        state.memory.set(2, M68K::DataSize::SIZE_WORD, 0x0004); // src offset
        state.memory.set(4, M68K::DataSize::SIZE_WORD, 0xFFFE); // dest offset

        state.registers.set(M68K::REG_A0, M68K::DataSize::SIZE_LONG, 0x1000);
        state.registers.set(M68K::REG_A1, M68K::DataSize::SIZE_LONG, 0x2000);

        state.memory.set(0x1004, M68K::DataSize::SIZE_WORD, 0xCCDD); // dest offset

        instruction.get()->execute(state);
        uint32_t return_data = state.memory.get(0x1ffe, M68K::DataSize::SIZE_WORD);
        TEST_TRUE(return_data == 0xCCDD);
    }

    {
        TEST_LABEL("addr index word - move.w D0, (8, A0, D1.W)");
        auto instruction = M68K::INSTRUCTION::Move::create(0x3180); // move.w D0, (8, A0, D1.W)
        M68K::CPUState state = M68K::CPUState();
        
        state.memory.set(0, M68K::DataSize::SIZE_WORD, 0x3180); // opcode
        state.memory.set(2, M68K::DataSize::SIZE_WORD, 0x1008); // index (8, A0, D1.W)

        state.registers.set(M68K::REG_D0, M68K::DataSize::SIZE_LONG, 0xAABBCCDD);
        state.registers.set(M68K::REG_D1, M68K::DataSize::SIZE_LONG, 0xFFFFFFFE); // -2
        state.registers.set(M68K::REG_A0, M68K::DataSize::SIZE_LONG, 0x1000);

        instruction.get()->execute(state);
        uint32_t return_data = state.memory.get(0x1006, M68K::DataSize::SIZE_WORD);
        TEST_TRUE(return_data == 0xCCDD);
    }

    {
        TEST_LABEL("addr index long - move.l D0, (8, A0, D1.L)");
        auto instruction = M68K::INSTRUCTION::Move::create(0x2180); // move.l D0, (8, A0, D1.L)
        M68K::CPUState state = M68K::CPUState();
        
        state.memory.set(0, M68K::DataSize::SIZE_WORD, 0x2180); // opcode
        state.memory.set(2, M68K::DataSize::SIZE_WORD, 0x1808); // index (8, A0, D1.L)

        state.registers.set(M68K::REG_D0, M68K::DataSize::SIZE_LONG, 0xAABBCCDD);
        state.registers.set(M68K::REG_D1, M68K::DataSize::SIZE_LONG, 0xFFFFFFFE); // -2
        state.registers.set(M68K::REG_A0, M68K::DataSize::SIZE_LONG, 0x1000);

        instruction.get()->execute(state);
        uint32_t return_data = state.memory.get(0x1006, M68K::DataSize::SIZE_LONG);
        TEST_TRUE(return_data == 0xAABBCCDD);
    }

    {
        TEST_LABEL("pc displacement - move.w (PC, 6), D0");
        auto instruction = M68K::INSTRUCTION::Move::create(0x303A); // move.w (PC, 6), D0
        M68K::CPUState state = M68K::CPUState();
        
        state.memory.set(0, M68K::DataSize::SIZE_WORD, 0x303A); // opcode
        state.memory.set(2, M68K::DataSize::SIZE_WORD, 0x0004); // src offset - 2
        state.memory.set(4, M68K::DataSize::SIZE_WORD, 0x0000); // padding
        state.memory.set(6, M68K::DataSize::SIZE_WORD, 0xCCDD); // data

        instruction.get()->execute(state);
        uint32_t return_data = state.registers.get(M68K::REG_D0, M68K::DataSize::SIZE_WORD);
        TEST_TRUE(return_data == 0xCCDD);
    }

    {
        TEST_LABEL("pc displacement negative - move.w (PC, -2), D0");
        auto instruction = M68K::INSTRUCTION::Move::create(0x303A); // move.w (PC, -2), D0
        M68K::CPUState state = M68K::CPUState();
        
        state.memory.set(0, M68K::DataSize::SIZE_WORD, 0x303A); // opcode
        state.memory.set(2, M68K::DataSize::SIZE_WORD, 0xFFFE); // src offset - 2

        instruction.get()->execute(state);
        uint32_t return_data = state.registers.get(M68K::REG_D0, M68K::DataSize::SIZE_WORD);
        TEST_TRUE(return_data == 0x303A);
    }

    {
        TEST_LABEL("absolute short - move.w D0, $1000");
        auto instruction = M68K::INSTRUCTION::Move::create(0x31C0); // move.w D0, $1000
        M68K::CPUState state = M68K::CPUState();
        
        state.registers.set(M68K::REG_D0, M68K::DataSize::SIZE_LONG, 0xAABBCCDD);
        state.memory.set(0, M68K::DataSize::SIZE_WORD, 0x31C0); // opcode
        state.memory.set(2, M68K::DataSize::SIZE_WORD, 0x1000); // dest word

        instruction.get()->execute(state);
        uint32_t return_data = state.memory.get(0x1000, M68K::DataSize::SIZE_WORD);
        TEST_TRUE(return_data == 0xCCDD);
    }

    {
        TEST_LABEL("absolute long - move.w D0, $10000");
        auto instruction = M68K::INSTRUCTION::Move::create(0x33C0); // move.w D0, $10000
        M68K::CPUState state = M68K::CPUState();
        
        state.registers.set(M68K::REG_D0, M68K::DataSize::SIZE_LONG, 0xAABBCCDD);
        state.memory.set(0, M68K::DataSize::SIZE_WORD, 0x33C0); // opcode
        state.memory.set(2, M68K::DataSize::SIZE_LONG, 0x10000); // dest long

        instruction.get()->execute(state);
        uint32_t return_data = state.memory.get(0x10000, M68K::DataSize::SIZE_WORD);
        TEST_TRUE(return_data == 0xCCDD);
    }

    {
        TEST_LABEL("immediate - move.w $AABB, D0");
        auto instruction = M68K::INSTRUCTION::Move::create(0x303c); // move.w $AABB, D0
        M68K::CPUState state = M68K::CPUState();
        
        state.memory.set(0, M68K::DataSize::SIZE_WORD, 0x303c); // opcode
        state.memory.set(2, M68K::DataSize::SIZE_WORD, 0xCCDD); // dest long

        instruction.get()->execute(state);
        uint32_t return_data = state.registers.get(M68K::REG_D0, M68K::DataSize::SIZE_WORD);
        TEST_TRUE(return_data == 0xCCDD);
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
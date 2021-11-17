#include "tests_functions.hpp"

#include "m68k.hpp"
#include "memory.hpp"
#include "helpers.hpp"

using namespace M68K;

int main(int, char**){
    TEST_NAME("Addressing modes");
    
    {
        TEST_LABEL("direct - move.w A1, D1");
        auto instruction = INSTRUCTION::Move::create(0x3209); // move.w A1, D1
        CPUState state = CPUState();
        
        state.registers.set(REG_A1, DataSize::SIZE_LONG, 0xAABBCCDD);

        instruction.get()->execute(state);
        uint32_t return_data = state.registers.get(REG_D1, DataSize::SIZE_LONG);
        TEST_TRUE(return_data == 0xCCDD);
    }

    {
        TEST_LABEL("addr direct - movea.l D1, A0");
        auto instruction = INSTRUCTION::Move::create(0x2041); // movea.l D1, A0
        CPUState state = CPUState();
        
        state.registers.set(REG_D1, DataSize::SIZE_LONG, 0xAABBCCDD);

        instruction.get()->execute(state);
        uint32_t return_data = state.registers.get(REG_A0, DataSize::SIZE_LONG);
        TEST_TRUE(return_data == 0xAABBCCDD);
    }
    
    {
        TEST_LABEL("addr indirect - move.l D1, (A0)");
        auto instruction = INSTRUCTION::Move::create(0x2081); // move.l D1, (A0)
        CPUState state = CPUState();
        
        state.registers.set(REG_D1, DataSize::SIZE_LONG, 0xAABBCCDD);
        state.registers.set(REG_A0, DataSize::SIZE_LONG, 0x1000);

        instruction.get()->execute(state);
        uint32_t return_data = state.memory.get(0x1000, DataSize::SIZE_LONG);
        TEST_TRUE(return_data == 0xAABBCCDD);
    }

    {
        TEST_LABEL("addr indirect++ - move.l D1, (A0)+");
        auto instruction = INSTRUCTION::Move::create(0x20C1); // move.l D1, (A0)+
        CPUState state = CPUState();
        
        state.registers.set(REG_D1, DataSize::SIZE_LONG, 0xAABBCCDD);
        state.registers.set(REG_A0, DataSize::SIZE_LONG, 0x1000);

        instruction.get()->execute(state);
        uint32_t return_data = state.registers.get(REG_A0, DataSize::SIZE_LONG);
        TEST_TRUE(return_data == 0x1004);
    }

    {
        TEST_LABEL("addr --indirect - move.l D1, -(A0)");
        auto instruction = INSTRUCTION::Move::create(0x2101); // move.l D1, -(A0)
        CPUState state = CPUState();
        
        state.registers.set(REG_D1, DataSize::SIZE_LONG, 0xAABBCCDD);
        state.registers.set(REG_A0, DataSize::SIZE_LONG, 0x1000);

        instruction.get()->execute(state);
        uint32_t return_data = state.registers.get(REG_A0, DataSize::SIZE_LONG);
        TEST_TRUE(return_data == 0x0ffc);
    }

    {
        TEST_LABEL("addr displacement - move.w (A0, 4), (A1, 8)");
        auto instruction = INSTRUCTION::Move::create(0x3368); // move.w (A0, 4), (A1, 8)
        CPUState state = CPUState();
        
        state.memory.set(0, DataSize::SIZE_WORD, 0x3368); // opcode
        state.memory.set(2, DataSize::SIZE_WORD, 0x0004); // src offset
        state.memory.set(4, DataSize::SIZE_WORD, 0x0008); // dest offset

        state.registers.set(REG_A0, DataSize::SIZE_LONG, 0x1000);
        state.registers.set(REG_A1, DataSize::SIZE_LONG, 0x2000);

        state.memory.set(0x1004, DataSize::SIZE_WORD, 0xCCDD); // dest offset

        instruction.get()->execute(state);
        uint32_t return_data = state.memory.get(0x2008, DataSize::SIZE_WORD);
        TEST_TRUE(return_data == 0xCCDD);
    }

    {
        TEST_LABEL("addr displacement negative - move.w (A0, 4), (A1, -2)");
        auto instruction = INSTRUCTION::Move::create(0x3368); // move.w (A0, 4), (A1, -2)
        CPUState state = CPUState();
        
        state.memory.set(0, DataSize::SIZE_WORD, 0x3368); // opcode
        state.memory.set(2, DataSize::SIZE_WORD, 0x0004); // src offset
        state.memory.set(4, DataSize::SIZE_WORD, 0xFFFE); // dest offset

        state.registers.set(REG_A0, DataSize::SIZE_LONG, 0x1000);
        state.registers.set(REG_A1, DataSize::SIZE_LONG, 0x2000);

        state.memory.set(0x1004, DataSize::SIZE_WORD, 0xCCDD); // dest offset

        instruction.get()->execute(state);
        uint32_t return_data = state.memory.get(0x1ffe, DataSize::SIZE_WORD);
        TEST_TRUE(return_data == 0xCCDD);
    }

    {
        TEST_LABEL("addr index word - move.w D0, (8, A0, D1.W)");
        auto instruction = INSTRUCTION::Move::create(0x3180); // move.w D0, (8, A0, D1.W)
        CPUState state = CPUState();
        
        state.memory.set(0, DataSize::SIZE_WORD, 0x3180); // opcode
        state.memory.set(2, DataSize::SIZE_WORD, 0x1008); // index (8, A0, D1.W)

        state.registers.set(REG_D0, DataSize::SIZE_LONG, 0xAABBCCDD);
        state.registers.set(REG_D1, DataSize::SIZE_LONG, 0xFFFFFFFE); // -2
        state.registers.set(REG_A0, DataSize::SIZE_LONG, 0x1000);

        instruction.get()->execute(state);
        uint32_t return_data = state.memory.get(0x1006, DataSize::SIZE_WORD);
        TEST_TRUE(return_data == 0xCCDD);
    }

    {
        TEST_LABEL("addr index long - move.l D0, (8, A0, D1.L)");
        auto instruction = INSTRUCTION::Move::create(0x2180); // move.l D0, (8, A0, D1.L)
        CPUState state = CPUState();
        
        state.memory.set(0, DataSize::SIZE_WORD, 0x2180); // opcode
        state.memory.set(2, DataSize::SIZE_WORD, 0x1808); // index (8, A0, D1.L)

        state.registers.set(REG_D0, DataSize::SIZE_LONG, 0xAABBCCDD);
        state.registers.set(REG_D1, DataSize::SIZE_LONG, 0xFFFFFFFE); // -2
        state.registers.set(REG_A0, DataSize::SIZE_LONG, 0x1000);

        instruction.get()->execute(state);
        uint32_t return_data = state.memory.get(0x1006, DataSize::SIZE_LONG);
        TEST_TRUE(return_data == 0xAABBCCDD);
    }

    // {
    //     TEST_LABEL("pc displacement - move.w (PC, 6), D0");
    //     auto instruction = INSTRUCTION::Move::create(0x303A); // move.w (PC, 6), D0
    //     CPUState state = CPUState();
        
    //     state.memory.set(0, DataSize::SIZE_WORD, 0x303A); // opcode
    //     state.memory.set(2, DataSize::SIZE_WORD, 0x0004); // src offset - 2
    //     state.memory.set(4, DataSize::SIZE_WORD, 0x0000); // padding
    //     state.memory.set(6, DataSize::SIZE_WORD, 0xCCDD); // data

    //     instruction.get()->execute(state);
    //     uint32_t return_data = state.registers.get(REG_D0, DataSize::SIZE_WORD);
    //     TEST_TRUE(return_data == 0xCCDD);
    // }

    // {
    //     TEST_LABEL("pc displacement negative - move.w (PC, -2), D0");
    //     auto instruction = INSTRUCTION::Move::create(0x303A); // move.w (PC, -2), D0
    //     CPUState state = CPUState();
        
    //     state.memory.set(0, DataSize::SIZE_WORD, 0x303A); // opcode
    //     state.memory.set(2, DataSize::SIZE_WORD, 0xFFFE); // src offset - 2

    //     instruction.get()->execute(state);
    //     uint32_t return_data = state.registers.get(REG_D0, DataSize::SIZE_WORD);
    //     TEST_TRUE(return_data == 0x303A);
    // }

    {
        TEST_LABEL("absolute short - move.w D0, $1000");
        auto instruction = INSTRUCTION::Move::create(0x31C0); // move.w D0, $1000
        CPUState state = CPUState();
        
        state.registers.set(REG_D0, DataSize::SIZE_LONG, 0xAABBCCDD);
        state.memory.set(0, DataSize::SIZE_WORD, 0x31C0); // opcode
        state.memory.set(2, DataSize::SIZE_WORD, 0x1000); // dest word

        instruction.get()->execute(state);
        uint32_t return_data = state.memory.get(0x1000, DataSize::SIZE_WORD);
        TEST_TRUE(return_data == 0xCCDD);
    }

    {
        TEST_LABEL("absolute long - move.w D0, $10000");
        auto instruction = INSTRUCTION::Move::create(0x33C0); // move.w D0, $10000
        CPUState state = CPUState();
        
        state.registers.set(REG_D0, DataSize::SIZE_LONG, 0xAABBCCDD);
        state.memory.set(0, DataSize::SIZE_WORD, 0x33C0); // opcode
        state.memory.set(2, DataSize::SIZE_LONG, 0x10000); // dest long

        instruction.get()->execute(state);
        uint32_t return_data = state.memory.get(0x10000, DataSize::SIZE_WORD);
        TEST_TRUE(return_data == 0xCCDD);
    }

    {
        TEST_LABEL("immediate - move.w $AABB, D0");
        auto instruction = INSTRUCTION::Move::create(0x303c); // move.w $AABB, D0
        CPUState state = CPUState();
        
        state.memory.set(0, DataSize::SIZE_WORD, 0x303c); // opcode
        state.memory.set(2, DataSize::SIZE_WORD, 0xCCDD); // dest long

        instruction.get()->execute(state);
        uint32_t return_data = state.registers.get(REG_D0, DataSize::SIZE_WORD);
        TEST_TRUE(return_data == 0xCCDD);
    }
}
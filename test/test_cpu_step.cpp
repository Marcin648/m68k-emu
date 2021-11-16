#include "tests_functions.hpp"
#include "m68k.hpp"

using namespace M68K;

int main(int, char**){
    TEST_NAME("CPU step");
    
    CPU cpu = CPU();
    {
        TEST_LABEL("moveq, moveq, addl");
        // 0:   7064            moveq #100,%d0
        // 2:   7264            moveq #100,%d1
        // 4:   d280            addl %d0,%d1

        uint32_t return_data;

        cpu.state.memory.set(0x1000, DataSize::SIZE_WORD, 0x7064); // moveq #100,%d0
        cpu.state.memory.set(0x1002, DataSize::SIZE_WORD, 0x7264); // moveq #100,%d1
        cpu.state.memory.set(0x1004, DataSize::SIZE_WORD, 0xD280); // addl %d0,%d1
        cpu.state.registers.set(REG_PC, SIZE_LONG, 0x1000);

        cpu.step();
        return_data = cpu.state.registers.get(REG_D0, DataSize::SIZE_LONG);
        TEST_TRUE(return_data == 100);

        cpu.step();
        return_data = cpu.state.registers.get(REG_D1, DataSize::SIZE_LONG);
        TEST_TRUE(return_data == 100);

        cpu.step();
        return_data = cpu.state.registers.get(REG_D1, DataSize::SIZE_LONG);
        TEST_TRUE(return_data == 200);
    }
}

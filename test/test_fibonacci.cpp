#include "tests_functions.hpp"
#include "m68k.hpp"

using namespace M68K;

int main(int, char**){
    TEST_NAME("Program fibonacci");
    
    {
        TEST_LABEL("fibonacci");
        CPU cpu = CPU();
        cpu.loadELF("../../test/binary/fibonacci.elf");

        while(cpu.state.registers.get(REG_PC, SIZE_LONG) != 0x10058){
            cpu.step();
        }

        uint32_t data_ptr = 0x4000;
        uint32_t data = cpu.state.memory.get(data_ptr, SIZE_LONG);

        TEST_TRUE(data == 6765);
    }
}

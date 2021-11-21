#include "tests_functions.hpp"
#include "m68k.hpp"

#include <cstdio>

using namespace M68K;

int main(int, char**){
    TEST_NAME("Program bubblesort");
    
    {
        TEST_LABEL("bubblesort");
        CPU cpu = CPU();
        cpu.loadELF("../../test/binary/bubblesort.elf");

        for(;;){
            cpu.step();
        }

        uint32_t return_data = cpu.state.registers.get(REG_D1, SIZE_LONG);
        TEST_TRUE(return_data == 20000);
    }
}

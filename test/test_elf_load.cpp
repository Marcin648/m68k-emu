#include "tests_functions.hpp"
#include "m68k.hpp"

#include <cstdio>

using namespace M68K;

int main(int, char**){
    TEST_NAME("ELF Load");
    
    {
        TEST_LABEL("10000 + 10000");
        CPU cpu = CPU();
        cpu.loadELF("../../test/files/elf_test.elf");

        for(size_t i = 0; i < 6; i++){
            cpu.step();
        }

        uint32_t return_data = cpu.state.registers.get(REG_D1, SIZE_LONG);
        TEST_TRUE(return_data == 20000);
    }
}

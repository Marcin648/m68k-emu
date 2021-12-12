#include "tests_functions.hpp"
#include "m68k.hpp"

using namespace M68K;

int main(int, char**){
    TEST_NAME("Program bubblesort");
    
    {
        TEST_LABEL("bubblesort");
        CPU cpu = CPU();
        cpu.loadELF("../../test/binary/bubblesort.c.elf");

        while(cpu.state.registers.get(REG_PC, SIZE_LONG) != 0x100c4){
            cpu.step();
        }

        uint32_t data_ptr = 0x3000;
        uint32_t last_data = 0;
        for(size_t i = 0; i < 30; i++){
            uint32_t data_addr = data_ptr + (i * SIZE_LONG);
            uint32_t data = cpu.state.memory.get(data_addr, SIZE_LONG);
            TEST_TRUE(last_data <= data);
            last_data = data;
        }
    }
}

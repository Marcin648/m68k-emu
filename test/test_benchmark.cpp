#include "tests_functions.hpp"
#include "m68k.hpp"

#include <chrono>

using namespace M68K;

double timeInterval(void){
    static auto last_time = std::chrono::high_resolution_clock::now();
    auto time_now = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> dt = time_now - last_time;
    last_time = time_now;
    return dt.count() / 1000.0;
}

int main(int, char**){
    TEST_NAME("Program benchmark");
    
    {
        TEST_LABEL("benchmark");
        CPU cpu = CPU();
        cpu.loadELF("../../test/binary/benchmark.elf");

        size_t n = 0;
        timeInterval();
        while(cpu.state.registers.get(REG_PC, SIZE_LONG) != 0x101A4){
            cpu.step();
            n++;
        }
        double dtime = timeInterval();

        std::cout << "Execute " << n << " instructions in " << dtime << " sec." << std::endl;
        std::cout << "Frequency: " << ((double)n/dtime)/1000.0 << "kHz" << std::endl;
    }
}

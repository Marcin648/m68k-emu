#include <iostream>
#include "m68k.hpp"
#include "memory.hpp"
#include "helpers.hpp"


int main(int, char**){
    M68K::CPU cpu;
    int a = 128;
    int b = M68K::LSL(a, 1);
    int c = M68K::ROL_8(a, 1);
    std::cout << a << " " << b << " " << c << std::endl;

    M68K::Memory memory;

    memory.set(0, M68K::DataSize::SIZE_LONG, 0xAABBCCDD);

    uint32_t data1 = memory.get(0, M68K::DataSize::SIZE_LONG);
    uint32_t data2 = memory.get(0, M68K::DataSize::SIZE_WORD);
    uint32_t data3 = memory.get(0, M68K::DataSize::SIZE_BYTE);

    return 0;
}

#include "tests_functions.hpp"

#include "m68k.hpp"
#include "memory.hpp"
#include "helpers.hpp"

int main(int, char**){
    M68K::Memory memory;
    memory.set(0, M68K::DataSize::SIZE_LONG, 0xAABBCCDD);

    uint32_t data1 = memory.get(0, M68K::DataSize::SIZE_LONG);
    uint32_t data2 = memory.get(0, M68K::DataSize::SIZE_WORD);
    uint32_t data3 = memory.get(0, M68K::DataSize::SIZE_BYTE);

    TEST_TRUE(data1 == 0xAABBCCDD);
    TEST_TRUE(data2 == 0xAABB);
    TEST_TRUE(data3 == 0xAA);

    TEST_THROW(
        std::out_of_range,
        {
            memory.set(0xFFFFFE, M68K::DataSize::SIZE_LONG, 0xAABBCCDD);
        }
    );
}
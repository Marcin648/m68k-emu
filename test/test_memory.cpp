#include "tests_functions.hpp"

#include "m68k.hpp"
#include "memory.hpp"
#include "helpers.hpp"

int main(int, char**){
    TEST_NAME("memory");

    M68K::Memory memory;

    // Data store
    TEST_LABEL("set long");
    TEST_NO_THROW({
        memory.set(0x00, M68K::DataSize::SIZE_LONG, 0xAABBCCDD);
    });

    TEST_LABEL("set word");
    TEST_NO_THROW({
        memory.set(0x00, M68K::DataSize::SIZE_WORD, 0xAABB);
    });

    TEST_LABEL("set byte");
    TEST_NO_THROW({
        memory.set(0x00, M68K::DataSize::SIZE_BYTE, 0xAA);
    });

    TEST_LABEL("set long odd address");
    TEST_THROW(
        std::length_error,
        {
            memory.set(0x01, M68K::DataSize::SIZE_LONG, 0xAABBCCDD);
        }
    );

    TEST_LABEL("set word odd address");
    TEST_THROW(
        std::length_error,
        {
            memory.set(0x01, M68K::DataSize::SIZE_WORD, 0xAABB);
        }
    );

    TEST_LABEL("set byte odd address");
    TEST_NO_THROW(
        {
            memory.set(0x01, M68K::DataSize::SIZE_BYTE, 0xAA);
        }
    );

    TEST_LABEL("set out of range long");
    TEST_THROW(
        std::out_of_range,
        {
            memory.set(0xFFFFFE, M68K::DataSize::SIZE_LONG, 0xAABBCCDD);
        }
    );

    TEST_LABEL("set out of range word");
    TEST_THROW(
        std::out_of_range,
        {
            memory.set(0xFFFFFE, M68K::DataSize::SIZE_WORD, 0xAABB);
        }
    );
    
    // Data load
    uint32_t data;
    memory.set(0x00, M68K::DataSize::SIZE_LONG, 0xAABBCCDD);

    TEST_LABEL("get long");
    data = memory.get(0x00, M68K::DataSize::SIZE_LONG);
    TEST_TRUE(data == 0xAABBCCDD);

    TEST_LABEL("get word");
    data = memory.get(0x00, M68K::DataSize::SIZE_WORD);
    TEST_TRUE(data == 0xAABB);

    TEST_LABEL("get byte");
    data = memory.get(0x00, M68K::DataSize::SIZE_BYTE);
    TEST_TRUE(data == 0xAA);

    TEST_LABEL("get long odd address");
    TEST_THROW(
        std::length_error,
        {
            data = memory.get(0x01, M68K::DataSize::SIZE_LONG);
        }
    );

    TEST_LABEL("get word odd address");
    TEST_THROW(
        std::length_error,
        {
            data = memory.get(0x01, M68K::DataSize::SIZE_WORD);
        }
    );

    TEST_LABEL("get byte odd address");
    TEST_NO_THROW(
        {
            data = memory.get(0x01, M68K::DataSize::SIZE_BYTE);
        }
    );

    TEST_LABEL("get out of range long");
    TEST_THROW(
        std::out_of_range,
        {
            data = memory.get(0xFFFFFE, M68K::DataSize::SIZE_LONG);
        }
    );

    TEST_LABEL("get out of range word");
    TEST_THROW(
        std::out_of_range,
        {
            data = memory.get(0xFFFFFE, M68K::DataSize::SIZE_WORD);
        }
    );
}
#include <catch2/catch_test_macros.hpp>
#include <stdexcept>

#include "m68k.hpp"
#include "memory.hpp"
#include "helpers.hpp"

using namespace M68K;

// Data store

TEST_CASE("Memory: set long", "[memory]") {
    Memory memory;
    CHECK_NOTHROW(memory.set(0x00, DataSize::SIZE_LONG, 0xAABBCCDD));
}

TEST_CASE("Memory: set word", "[memory]") {
    Memory memory;
    CHECK_NOTHROW(memory.set(0x00, DataSize::SIZE_WORD, 0xAABB));
}

TEST_CASE("Memory: set byte", "[memory]") {
    Memory memory;
    CHECK_NOTHROW(memory.set(0x00, DataSize::SIZE_BYTE, 0xAA));
}

TEST_CASE("Memory: set long odd address", "[memory]") {
    Memory memory;
    CHECK_THROWS_AS(memory.set(0x01, DataSize::SIZE_LONG, 0xAABBCCDD), std::length_error);
}

TEST_CASE("Memory: set word odd address", "[memory]") {
    Memory memory;
    CHECK_THROWS_AS(memory.set(0x01, DataSize::SIZE_WORD, 0xAABB), std::length_error);
}

TEST_CASE("Memory: set byte odd address", "[memory]") {
    Memory memory;
    CHECK_NOTHROW(memory.set(0x01, DataSize::SIZE_BYTE, 0xAA));
}

TEST_CASE("Memory: set out of range long", "[memory]") {
    Memory memory;
    CHECK_THROWS_AS(memory.set(0xFFFFFE, DataSize::SIZE_LONG, 0xAABBCCDD), std::out_of_range);
}

// TEST_CASE("Memory: set out of range word", "[memory]") { // it's not posible
//     Memory memory;
//     CHECK_THROWS_AS(memory.set(0xFFFFFF, DataSize::SIZE_WORD, 0xAABB), std::out_of_range);
// }

// Data load

struct LoadedMemory {
    Memory memory;

    LoadedMemory(){
        memory.set(0x00, DataSize::SIZE_LONG, 0xAABBCCDD);
    }
};

TEST_CASE_METHOD(LoadedMemory, "Memory: get long", "[memory]") {
    uint32_t data = memory.get(0x00, DataSize::SIZE_LONG);
    CHECK(data == 0xAABBCCDD);
}

TEST_CASE_METHOD(LoadedMemory, "Memory: get word", "[memory]") {
    uint32_t data = memory.get(0x00, DataSize::SIZE_WORD);
    CHECK(data == 0xAABB);
}

TEST_CASE_METHOD(LoadedMemory, "Memory: get byte", "[memory]") {
    uint32_t data = memory.get(0x00, DataSize::SIZE_BYTE);
    CHECK(data == 0xAA);
}

TEST_CASE_METHOD(LoadedMemory, "Memory: get long odd address", "[memory]") {
    CHECK_THROWS_AS(memory.get(0x01, DataSize::SIZE_LONG), std::length_error);
}

TEST_CASE_METHOD(LoadedMemory, "Memory: get word odd address", "[memory]") {
    CHECK_THROWS_AS(memory.get(0x01, DataSize::SIZE_WORD), std::length_error);
}

TEST_CASE_METHOD(LoadedMemory, "Memory: get byte odd address", "[memory]") {
    CHECK_NOTHROW(memory.get(0x01, DataSize::SIZE_BYTE));
}

TEST_CASE_METHOD(LoadedMemory, "Memory: get out of range long", "[memory]") {
    CHECK_THROWS_AS(memory.get(0xFFFFFE, DataSize::SIZE_LONG), std::out_of_range);
}

// TEST_CASE_METHOD(LoadedMemory, "Memory: get out of range word", "[memory]") { // it's not posible
//     CHECK_THROWS_AS(memory.get(0xFFFFFF, DataSize::SIZE_WORD), std::out_of_range);
// }

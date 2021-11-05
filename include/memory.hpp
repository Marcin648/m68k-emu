#pragma once
#include <cstdint>
#include <vector>

#include "defines.hpp"

namespace M68K{
    class Memory{
    private:
        std::vector<uint8_t> memory = std::vector<uint8_t>(MEMORY_SIZE);
    public:
        Memory() = default;
        Memory(Memory&) = default;

        uint32_t get(std::size_t address, DataSize size);
        void set(std::size_t address, DataSize size, uint32_t data);
    };
}

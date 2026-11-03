#include "memory.hpp"
#include <algorithm>
#include <stdexcept>
#include <string>
using namespace M68K;

void Memory::throw_out_of_range(std::size_t address) {
    throw std::out_of_range(
        "Memory address out of range. " +
        std::to_string(address) + ">" + std::to_string(MEMORY_SIZE)
    ); //TODO: Throw special exception
}

void Memory::throw_invalid_size(DataSize size) {
    throw std::length_error(
        "Invalid memory request size. " +
        std::to_string(size)
    ); //TODO: Throw special exception
}

void Memory::load(std::size_t address, std::span<const uint8_t> data) {
    address = MASK_ADDR(address);
    if ((address > this->memory.size()) || (data.size() > this->memory.size() - address)) {
        throw_out_of_range(address);
    }
    std::copy(data.begin(), data.end(), this->memory.begin() + address);
}

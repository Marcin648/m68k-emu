#include "memory.hpp"
#include <algorithm>
#include <stdexcept>
#include <string>
using namespace M68K;

void Memory::throwUnaligned(){
    throw std::length_error("Memory address must be even."); //TODO: Throw special exception
}

void Memory::throwOutOfRange(std::size_t address){
    throw std::out_of_range(
        "Memory address out of range. " +
        std::to_string(address) + ">" + std::to_string(MEMORY_SIZE)
    ); //TODO: Throw special exception
}

void Memory::throwInvalidSize(DataSize size){
    throw std::length_error(
        "Invalid memory request size. " +
        std::to_string(size)
    ); //TODO: Throw special exception
}

void Memory::load(std::size_t address, std::span<const uint8_t> data){
    address = MASK_ADDR(address);
    if((address > this->memory.size()) || (data.size() > this->memory.size() - address)){
        throwOutOfRange(address);
    }
    std::copy(data.begin(), data.end(), this->memory.begin() + address);
}

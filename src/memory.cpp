#include "memory.hpp"
#include "helpers.hpp"
#include <stdexcept>
using namespace M68K;

uint32_t Memory::get(std::size_t address, DataSize size){
    address = MASK_ADDR(address);
    if((size != DataSize::SIZE_BYTE) && (address % 2 != 0)){ // only even is valid in word and long mode
        throw std::length_error("Memory address must be even."); //TODO: Throw special exception
    }
    
    if(address + size > this->memory.size()){ //memory overflow
        throw std::out_of_range(
            "Memory address out of range. " +
            std::to_string(address) + ">" + std::to_string(this->memory.size())
        ); //TODO: Throw special exception
    }

    uint32_t output_data = 0;

    switch(size){
        case DataSize::SIZE_BYTE:{
            output_data = this->memory[address];
            break;
        }

        case DataSize::SIZE_WORD:{  //BIG ENDIAN
            output_data = this->memory[address];
            output_data = (output_data << 8) | this->memory[address + 1];
            break;
        }

        case DataSize::SIZE_LONG:{  //BIG ENDIAN
            output_data = this->memory[address];
            output_data = (output_data << 8) | this->memory[address + 1];
            output_data = (output_data << 8) | this->memory[address + 2];
            output_data = (output_data << 8) | this->memory[address + 3];
            break;
        }
        
        default:{
            throw std::length_error(
                "Invalid memory request size. " +
                std::to_string(size)
            ); //TODO: Throw special exception
            break;
        }
    }
    return output_data;
}

void Memory::set(std::size_t address, DataSize size, uint32_t data){
    address = MASK_ADDR(address);
    if((size != DataSize::SIZE_BYTE) && (address % 2 != 0)){ // only even is valid in word and long mode
        throw std::length_error("Memory address must be even."); //TODO: Throw special exception
    }
    
    if(address + size > this->memory.size()){ // memory out of range.
        throw std::out_of_range(
            "Memory address out of range. " +
            std::to_string(address) + ">" + std::to_string(MEMORY_SIZE)
        ); //TODO: Throw special exception
    }

    switch(size){
        case DataSize::SIZE_BYTE:{
            this->memory[address] = MASK_8(data);
            break;
        }

        case DataSize::SIZE_WORD:{  //BIG ENDIAN
            this->memory[address] = MASK_8(data >> 8);
            this->memory[address + 1] = MASK_8(data);
            break;
        }

        case DataSize::SIZE_LONG:{  //BIG ENDIAN
            this->memory[address] = MASK_8(data >> 24);
            this->memory[address + 1] = MASK_8(data >> 16);
            this->memory[address + 2] = MASK_8(data >> 8);
            this->memory[address + 3] = MASK_8(data);
            break;
        }
        
        default:{
            throw std::length_error(
                "Invalid memory request size. " +
                std::to_string(size)
            ); //TODO: Throw special exception
            break;
        }
    }
    return;
}
#pragma once
#include <cstdint>
#include <cstddef>
#include <span>
#include <vector>

#include "defines.hpp"
#include "helpers.hpp"

namespace M68K {
    class Memory {
    private:
        std::vector<uint8_t> memory = std::vector<uint8_t>(MEMORY_SIZE);

        [[noreturn]] static void throwUnaligned();
        [[noreturn]] static void throwOutOfRange(std::size_t address);
        [[noreturn]] static void throwInvalidSize(DataSize size);

        std::size_t checkAddress(std::size_t address, DataSize size) const {
            address = MASK_ADDR(address);
            if ((size != DataSize::SIZE_BYTE) && (address % 2 != 0)) { // only even is valid in word and long mode
                throwUnaligned();
            }
            if (address + size > this->memory.size()) { // memory overflow
                throwOutOfRange(address);
            }
            return address;
        }

    public:
        uint32_t get(std::size_t address, DataSize size) const {
            address = this->checkAddress(address, size);
            const uint8_t* p = this->memory.data() + address;

            switch (size) { // BIG ENDIAN
                case DataSize::SIZE_BYTE: return p[0];
                case DataSize::SIZE_WORD: return (uint32_t(p[0]) << 8) | p[1];
                case DataSize::SIZE_LONG: return (uint32_t(p[0]) << 24) | (uint32_t(p[1]) << 16) | (uint32_t(p[2]) << 8) | p[3];
            }
            throwInvalidSize(size);
        }

        void set(std::size_t address, DataSize size, uint32_t data) {
            address = this->checkAddress(address, size);
            uint8_t* p = this->memory.data() + address;

            switch (size) { // BIG ENDIAN
                case DataSize::SIZE_BYTE: {
                    p[0] = MASK_8(data);
                    break;
                }
                case DataSize::SIZE_WORD: {
                    p[0] = MASK_8(data >> 8);
                    p[1] = MASK_8(data);
                    break;
                }
                case DataSize::SIZE_LONG: {
                    p[0] = MASK_8(data >> 24);
                    p[1] = MASK_8(data >> 16);
                    p[2] = MASK_8(data >> 8);
                    p[3] = MASK_8(data);
                    break;
                }
                default: {
                    throwInvalidSize(size);
                }
            }
        }

        void load(std::size_t address, std::span<const uint8_t> data);
    };
}

#pragma once
#include <cstdint>
#include <cstddef>
#include <span>
#include <vector>

#include "defines.hpp"
#include "helpers.hpp"

namespace M68K {
    struct AddressError {
        bool pending = false;
        bool read = false;
        uint32_t address = 0;
    };

    class Memory {
    private:
        std::vector<uint8_t> memory = std::vector<uint8_t>(MEMORY_SIZE);

        [[noreturn]] static void throw_out_of_range(std::size_t address);
        [[noreturn]] static void throw_invalid_size(DataSize size);

        bool check_aligned(std::size_t address, DataSize size, bool read) {
            if ((size == DataSize::SIZE_BYTE) || (address % 2 == 0)) { // only even is valid in word and long mode
                return true;
            }
            if (!this->address_error.pending) {
                this->address_error = {true, read, static_cast<uint32_t>(MASK_ADDR(address))};
            }
            return false;
        }

        std::size_t check_address(std::size_t address, DataSize size) const {
            address = MASK_ADDR(address);
            if (address + size > this->memory.size()) { // memory overflow
                throw_out_of_range(address);
            }
            return address;
        }

    public:
        AddressError address_error;

        uint32_t get(std::size_t address, DataSize size) {
            if (!this->check_aligned(address, size, true)) [[unlikely]] {
                return 0;
            }
            address = this->check_address(address, size);
            const uint8_t* p = this->memory.data() + address;

            switch (size) { // BIG ENDIAN
                case DataSize::SIZE_BYTE: return p[0];
                case DataSize::SIZE_WORD: return (uint32_t(p[0]) << 8) | p[1];
                case DataSize::SIZE_LONG: return (uint32_t(p[0]) << 24) | (uint32_t(p[1]) << 16) | (uint32_t(p[2]) << 8) | p[3];
            }
            throw_invalid_size(size);
        }

        void set(std::size_t address, DataSize size, uint32_t data) {
            if (!this->check_aligned(address, size, false)) [[unlikely]] {
                return;
            }
            address = this->check_address(address, size);
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
                    throw_invalid_size(size);
                }
            }
        }

        void load(std::size_t address, std::span<const uint8_t> data);
    };
}

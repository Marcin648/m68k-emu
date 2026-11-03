#pragma once
#include <array>
#include <cstdint>
#include <cstddef>

#include "defines.hpp"
#include "helpers.hpp"

namespace M68K {
    enum RegisterType {
        REG_D0 = 0,
        REG_D1 = 1,
        REG_D2 = 2,
        REG_D3 = 3,
        REG_D4 = 4,
        REG_D5 = 5,
        REG_D6 = 6,
        REG_D7 = 7,
        REG_A0 = 8,
        REG_A1 = 9,
        REG_A2 = 10,
        REG_A3 = 11,
        REG_A4 = 12,
        REG_A5 = 13,
        REG_A6 = 14,
        REG_A7 = 15,
        REG_USP = 15,
        REG_SSP = 15,
        REG_PC = 16,
        REG_SR = 17
    };

    enum StatusRegisterFlag {
        SR_FLAG_CARRY = (1 << 0),
        SR_FLAG_OVERFLOW = (1 << 1),
        SR_FLAG_ZERO = (1 << 2),
        SR_FLAG_NEGATIVE = (1 << 3),
        SR_FLAG_EXTEND = (1 << 4),
        SR_FLAG_SUPERVISOR = (1 << 13),
        SR_FLAG_TRACE = (1 << 15)
    };

    constexpr uint32_t SR_MASK = 0xA71F;
    constexpr uint32_t SR_INTERRUPT_MASK = 0x0700;
    constexpr uint32_t SR_INTERRUPT_SHIFT = 8;
    constexpr uint32_t SR_RESET = 0x2700;

    class Registers {
    public:
        static constexpr std::size_t REG_BUFFER_SIZE = 18;

    private:
        std::array<uint32_t, REG_BUFFER_SIZE> reg_buffer{};
        uint32_t usp = 0;
        uint32_t ssp = 0;

        [[noreturn]] static void throw_invalid_register(RegisterType reg);
        [[noreturn]] static void throw_invalid_size(DataSize size);

        void write_sr(uint32_t value);

    public:
        Registers() {
            this->reg_buffer[REG_SR] = SR_RESET;
        }

        bool is_supervisor() const {
            return (this->reg_buffer[REG_SR] & SR_FLAG_SUPERVISOR) != 0;
        }

        uint32_t get_usp() const { return this->is_supervisor() ? this->usp : this->reg_buffer[REG_A7]; }
        uint32_t get_ssp() const { return this->is_supervisor() ? this->reg_buffer[REG_A7] : this->ssp; }
        void set_usp(uint32_t value) { (this->is_supervisor() ? this->usp : this->reg_buffer[REG_A7]) = value; }
        void set_ssp(uint32_t value) { (this->is_supervisor() ? this->reg_buffer[REG_A7] : this->ssp) = value; }

        uint32_t get(RegisterType reg, DataSize size = DataSize::SIZE_LONG) const {
            if (static_cast<std::size_t>(reg) >= REG_BUFFER_SIZE) {
                throw_invalid_register(reg);
            }

            uint32_t value = this->reg_buffer[reg];
            switch (size) {
                case DataSize::SIZE_BYTE: return MASK_8(value);
                case DataSize::SIZE_WORD: return MASK_16(value);
                case DataSize::SIZE_LONG: return value;
            }
            throw_invalid_size(size);
        }

        void set(RegisterType reg, DataSize size, uint32_t data) {
            if (static_cast<std::size_t>(reg) >= REG_BUFFER_SIZE) {
                throw_invalid_register(reg);
            }

            uint32_t reg_value = this->reg_buffer[reg];
            switch (size) {
                case DataSize::SIZE_BYTE: reg_value = MASK_ABOVE_8(reg_value) | MASK_8(data); break;
                case DataSize::SIZE_WORD: reg_value = MASK_ABOVE_16(reg_value) | MASK_16(data); break;
                case DataSize::SIZE_LONG: reg_value = data; break;
                default: throw_invalid_size(size);
            }

            if (reg == REG_SR) {
                this->write_sr(reg_value);
                return;
            }

            this->reg_buffer[reg] = reg_value;
        }

        bool get(StatusRegisterFlag flag) const {
            return (this->reg_buffer[REG_SR] & static_cast<uint32_t>(flag)) != 0;
        }

        void set(StatusRegisterFlag flag, bool value) {
            uint32_t flag_mask = static_cast<uint32_t>(flag);
            uint32_t sr_value = this->reg_buffer[REG_SR] & ~flag_mask;
            if (value) {
                sr_value |= flag_mask;
            }
            if (flag == SR_FLAG_SUPERVISOR) {
                this->write_sr(sr_value);
                return;
            }
            this->reg_buffer[REG_SR] = sr_value;
        }
    };
}

#include "registers.hpp"
#include <stdexcept>
#include <string>
using namespace M68K;

void Registers::throw_invalid_register(RegisterType reg) {
    throw std::out_of_range(
        "Register index out of range. " +
        std::to_string(reg) + ">=" + std::to_string(REG_BUFFER_SIZE)
    ); //TODO: Throw special exception
}

void Registers::throw_invalid_size(DataSize size) {
    throw std::length_error(
        "Invalid register request size. " +
        std::to_string(size)
    ); //TODO: Throw special exception
}

void Registers::write_sr(uint32_t value) {
    value &= SR_MASK;
    uint32_t old_value = this->reg_buffer[REG_SR];
    if ((old_value ^ value) & SR_FLAG_SUPERVISOR) {
        if (value & SR_FLAG_SUPERVISOR) {
            this->usp = this->reg_buffer[REG_A7];
            this->reg_buffer[REG_A7] = this->ssp;
        } else {
            this->ssp = this->reg_buffer[REG_A7];
            this->reg_buffer[REG_A7] = this->usp;
        }
    }
    this->reg_buffer[REG_SR] = value;
}

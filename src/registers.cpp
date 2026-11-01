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

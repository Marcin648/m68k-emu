#include "registers.hpp"
#include "helpers.hpp"
#include <stdexcept>
using namespace M68K;

uint32_t Registers::get(RegisterType reg, DataSize size){
    if(reg > this->reg_buffer.size()){
        throw std::out_of_range(
            "Register index out of range. " +
            std::to_string(reg) + ">" + std::to_string(this->reg_buffer.size())
        ); //TODO: Throw special exception
    }

    uint32_t output_data = this->reg_buffer.at(reg);

    switch(size){
        case DataSize::SIZE_BYTE:{
            output_data = MASK_8(output_data);
            break;
        }

        case DataSize::SIZE_WORD:{
            output_data = MASK_16(output_data);
            break;
        }

        case DataSize::SIZE_LONG:{
            //output_data = MASK_32(output_data);
            break;
        }
        
        default:{
            throw std::length_error(
                "Invalid register request size. " +
                std::to_string(size)
            ); //TODO: Throw special exception
            break;
        }
    }

    return output_data;
}

void Registers::set(RegisterType reg, DataSize size, uint32_t data){
    if(reg > this->reg_buffer.size()){
        throw std::out_of_range(
            "Register index out of range. " +
            std::to_string(reg) + ">" + std::to_string(this->reg_buffer.size())
        ); //TODO: Throw special exception
    }

    uint32_t reg_value = this->reg_buffer.at(reg);

    switch(size){
        case DataSize::SIZE_BYTE:{
            reg_value = MASK_ABOVE_8(reg_value) | MASK_8(data);
            break;
        }

        case DataSize::SIZE_WORD:{
            reg_value = MASK_ABOVE_16(reg_value) | MASK_16(data);
            break;
        }

        case DataSize::SIZE_LONG:{
            reg_value = MASK_ABOVE_32(reg_value) | MASK_32(data);
            break;
        }
        
        default:{
            throw std::length_error(
                "Invalid register request size. " +
                std::to_string(size)
            ); //TODO: Throw special exception
            break;
        }
    }

    if(reg == REG_SR){
        reg_value = MASK_16(reg_value);
    }

    this->reg_buffer.at(reg) = reg_value;
    return;
}

bool Registers::get(StatusRegisterFlag flag){
    uint32_t sr_value = this->reg_buffer.at(REG_SR);
    uint32_t flag_mask = uint32_t(flag);
    return (sr_value & flag_mask) ? true : false;
}

void Registers::set(StatusRegisterFlag flag, bool value){
    uint32_t sr_value = this->reg_buffer.at(REG_SR);
    uint32_t flag_mask = uint32_t(flag);
    sr_value &= ~flag_mask;
    if(value){
        sr_value |= flag_mask;
    }
    this->reg_buffer.at(REG_SR) = sr_value;
    return;
}
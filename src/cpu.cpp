#include "cpu.hpp"
#include "elfio/elfio.hpp"

#include <iostream>

using namespace M68K;

void CPU::step(){
    uint32_t pc = this->state.registers.get(REG_PC);
    uint16_t opcode = this->state.memory.get(pc, SIZE_WORD);

    auto instruction = this->instruction_decoder.Decode(opcode);
    //std::cout << typeid(*instruction).name() << std::endl;
    instruction->execute(this->state);
}

bool CPU::loadELF(const std::string& file_name){
    ELFIO::elfio elf_reader;
    if(!elf_reader.load(file_name)){
        return false;
    }

    if(
        (elf_reader.get_class() != ELFIO::ELFCLASS32) ||
        (elf_reader.get_encoding() != ELFIO::ELFDATA2MSB) ||
        (elf_reader.get_type() != ELFIO::ET_REL) ||
        (elf_reader.get_machine() != ELFIO::EM_68K)
    ){
        return false;
    }


    uint32_t base_address = elf_reader.get_entry(); // FIXME

    for(const auto section : elf_reader.sections){
        std::cout << section->get_name() << "\t"
                  << section->get_address() << "\t"
                  << section->get_size() << "\t"
                  << section->get_offset() << "\t"
                  << section->get_addr_align() << "\t"
                  << section->get_link() << "\t"
                  << section->get_flags() << "\t"
                  << section->get_info() << "\t"
                  << section->get_type() << std::endl;

        if(section->get_name() == ".text"){
            size_t size = section->get_size();
            auto data = section->get_data();
            for(size_t i = 0; i < size; i++){
                uint32_t addr = base_address + i;
                this->state.memory.set(addr, SIZE_BYTE, data[i]);
            }
            break;
        }
    }

    this->state.registers.set(REG_USP, SIZE_LONG, MEMORY_SIZE);
    this->state.registers.set(REG_PC, SIZE_LONG, base_address);
    return true;
}
#include "cpu.hpp"
#include "elfio/elfio.hpp"

#include <iostream>

using namespace M68K;

void CPU::step() {
    uint32_t pc = this->state.registers.get(REG_PC);
    uint16_t opcode = this->state.memory.get(pc, SIZE_WORD);

    auto instruction = this->instruction_decoder.decode(opcode);
    //std::cout << typeid(*instruction).name() << std::endl;
    instruction->execute(this->state);
}

bool CPU::load_elf(const std::string& file_name) {
    ELFIO::elfio elf_reader;
    if (!elf_reader.load(file_name)) {
        return false;
    }

    if (
        (elf_reader.get_class() != ELFIO::ELFCLASS32) ||
        (elf_reader.get_encoding() != ELFIO::ELFDATA2MSB) ||
        (elf_reader.get_type() != ELFIO::ET_EXEC) ||
        (elf_reader.get_machine() != ELFIO::EM_68K)
    ) {
        return false;
    }

    this->state = CPUState();

    uint32_t entry_address = elf_reader.get_entry();

    for (const auto& segment : elf_reader.sections) {
        // std::cout << segment->get_name() << "\t"
        //           << segment->get_address() << "\t"
        //           << segment->get_size() << "\t"
        //           << segment->get_offset() << "\t"
        //           << segment->get_addr_align() << "\t"
        //           << segment->get_link() << "\t"
        //           << segment->get_flags() << "\t"
        //           << segment->get_info() << "\t"
        //           << segment->get_type() << std::endl;

        if (segment->get_type() == ELFIO::PT_LOAD) {
            const auto* data = reinterpret_cast<const uint8_t*>(segment->get_data());
            this->state.memory.load(segment->get_address(), std::span(data, segment->get_size()));
        }
    }

    this->state.registers.set(REG_USP, SIZE_LONG, MEMORY_SIZE);
    this->state.registers.set(REG_PC, SIZE_LONG, entry_address);
    return true;
}

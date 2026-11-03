#include "cpu.hpp"
#include "elfio/elfio.hpp"

#include <iostream>

using namespace M68K;

void CPU::step() {
    if (this->state.interrupt_level || this->state.stopped || this->state.halted) [[unlikely]] {
        if (this->state.halted) {
            return;
        }
        if (this->take_interrupt()) {
            this->state.halted = this->state.memory.address_error.pending;
            return;
        }
        if (this->state.stopped) {
            return;
        }
    }

    bool trace = this->state.registers.get(SR_FLAG_TRACE);
    if (trace) [[unlikely]] {
        this->state.trace_pending = true;
    }

    uint32_t pc = this->state.registers.get(REG_PC);
    uint16_t opcode = this->state.memory.get(pc, SIZE_WORD);
    if (this->state.memory.address_error.pending) [[unlikely]] {
        this->state.address_error(true, 0);
        return;
    }

    auto instruction = this->instruction_decoder.decode(opcode);
    //std::cout << typeid(*instruction).name() << std::endl;
    instruction->execute(this->state);

    if (this->state.memory.address_error.pending) [[unlikely]] {
        this->state.address_error(false, opcode);
        return;
    }

    if (trace && this->state.trace_pending) [[unlikely]] {
        this->state.trace_pending = false;
        this->state.exception(VECTOR_TRACE);
        this->state.halted = this->state.memory.address_error.pending;
    }
}

bool CPU::take_interrupt() {
    uint8_t level = this->state.interrupt_level;
    uint32_t mask = (this->state.registers.get(REG_SR, SIZE_WORD) & SR_INTERRUPT_MASK) >> SR_INTERRUPT_SHIFT;
    if ((level == 0) || ((level != 7) && (level <= mask))) {
        return false;
    }

    this->state.interrupt_level = 0;
    this->state.exception(VECTOR_AUTOVECTOR + level);
    uint32_t sr = this->state.registers.get(REG_SR, SIZE_WORD) & ~SR_INTERRUPT_MASK;
    this->state.registers.set(REG_SR, SIZE_WORD, sr | (level << SR_INTERRUPT_SHIFT));
    return true;
}

void CPU::reset() {
    this->state.registers.set(REG_SR, SIZE_WORD, SR_RESET);
    this->state.registers.set_ssp(this->state.memory.get(VECTOR_RESET_SSP * 4, SIZE_LONG));
    this->state.registers.set(REG_PC, SIZE_LONG, this->state.memory.get(VECTOR_RESET_PC * 4, SIZE_LONG));
    this->state.stopped = false;
    this->state.halted = false;
    this->state.trace_pending = false;
    this->state.interrupt_level = 0;
}

void CPU::interrupt(uint8_t level) {
    this->state.interrupt_level = level & 0x7;
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

    this->state.registers.set_ssp(MEMORY_SIZE);
    this->state.registers.set(REG_PC, SIZE_LONG, entry_address);
    return true;
}

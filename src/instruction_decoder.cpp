#include "instruction_decoder.hpp"
#include "instructions.hpp"
#include <vector>
#include <functional>
#include <memory>
#include <cstdint>

using namespace M68K;

struct MaskTableElement{
    uint32_t mask;
    uint32_t value;
    std::function<std::shared_ptr<INSTRUCTION::Instruction>(uint16_t)> create_function;
};

std::vector<MaskTableElement> opcode_mask_table = {
    // {0xFFFF, 0x003C, INSTRUCTION::OriToCCR::create},   //(0b1111111111111111, 0b0000000000111100, "OriToCCR")
    // {0xFFFF, 0x007C, INSTRUCTION::OriToSR::create},    //(0b1111111111111111, 0b0000000001111100, "OriToSR")
    // {0xFFFF, 0x023C, INSTRUCTION::AndiToCCR::create},  //(0b1111111111111111, 0b0000001000111100, "AndiToCCR")
    // {0xFFFF, 0x027C, INSTRUCTION::AndiToSR::create},   //(0b1111111111111111, 0b0000001001111100, "AndiToSR")
    {0xFFFF, 0x4AFC, INSTRUCTION::Illegal::create},    //(0b1111111111111111, 0b0100101011111100, "Illegal")
    // {0xFFFF, 0x4E70, INSTRUCTION::Reset::create},      //(0b1111111111111111, 0b0100111001110000, "Reset")
    {0xFFFF, 0x4E71, INSTRUCTION::Nop::create},        //(0b1111111111111111, 0b0100111001110001, "Nop")
    // {0xFFFF, 0x4E72, INSTRUCTION::Stop::create},       //(0b1111111111111111, 0b0100111001110010, "Stop")
    // {0xFFFF, 0x4E73, INSTRUCTION::Rte::create},        //(0b1111111111111111, 0b0100111001110011, "Rte")
    {0xFFFF, 0x4E75, INSTRUCTION::Rts::create},        //(0b1111111111111111, 0b0100111001110101, "Rts")
    // {0xFFFF, 0x4E76, INSTRUCTION::Trapv::create},      //(0b1111111111111111, 0b0100111001110110, "Trapv")
    // {0xFFFF, 0x4E77, INSTRUCTION::Rtr::create},        //(0b1111111111111111, 0b0100111001110111, "Rtr")
    // {0xFFF8, 0x4840, INSTRUCTION::Swap::create},       //(0b1111111111111000, 0b0100100001000000, "Swap")
    {0xFFF8, 0x4E50, INSTRUCTION::Link::create},       //(0b1111111111111111, 0b0100111001010000, "Link")
    {0xFFF8, 0x4E58, INSTRUCTION::Unlk::create},       //(0b1111111111111000, 0b0100111001011000, "Unlk")
    // {0xFFB8, 0x4880, INSTRUCTION::Ext::create},        //(0b1111111110111000, 0b0100100010000000, "Ext")
    // {0xFFF0, 0x4E40, INSTRUCTION::Trap::create},       //(0b1111111111110000, 0b0100111001000000, "Trap")
    // {0xFFF0, 0x4E60, INSTRUCTION::MoveUSP::create},    //(0b1111111111110000, 0b0100111001100000, "MoveUSP")
    // {0xFFC0, 0x0800, INSTRUCTION::Btst::create},       //(0b1111111111000000, 0b0000100000000000, "Btst")
    // {0xFFC0, 0x0840, INSTRUCTION::Bchg::create},       //(0b1111111111000000, 0b0000100001000000, "Bchg")
    // {0xFFC0, 0x0880, INSTRUCTION::Bclr::create},       //(0b1111111111000000, 0b0000100010000000, "Bclr")
    // {0xFFC0, 0x08C0, INSTRUCTION::Bset::create},       //(0b1111111111000000, 0b0000100011000000, "Bset")
    // {0xFFC0, 0x40C0, INSTRUCTION::MoveFromSR::create}, //(0b1111111111000000, 0b0100000011000000, "MoveFromSR")
    // {0xFFC0, 0x44C0, INSTRUCTION::MoveToCCR::create},  //(0b1111111111000000, 0b0100010011000000, "MoveToCCR")
    // {0xFFC0, 0x46C0, INSTRUCTION::MoveToSR::create},   //(0b1111111111000000, 0b0100011011000000, "MoveToSR")
    // {0xFFC0, 0x4800, INSTRUCTION::Nbcd::create},       //(0b1111111111000000, 0b0100100000000000, "Nbcd")
    {0xFFC0, 0x4840, INSTRUCTION::Pea::create},        //(0b1111111111000000, 0b0100100001000000, "Pea")
    // {0xFFC0, 0x4AC0, INSTRUCTION::Tas::create},        //(0b1111111111000000, 0b0100101011000000, "Tas")
    {0xFFC0, 0x4E80, INSTRUCTION::Jsr::create},        //(0b1111111111000000, 0b0100111010000000, "Jsr")
    {0xFFC0, 0x4EC0, INSTRUCTION::Jmp::create},        //(0b1111111111000000, 0b0100111011000000, "Jmp")
    // {0xF0F8, 0x50C8, INSTRUCTION::Dbcc::create},       //(0b1111000011111000, 0b0101000011001000, "Dbcc")
    // {0xF1F0, 0x8100, INSTRUCTION::Sbcd::create},       //(0b1111000111110000, 0b1000000100000000, "Sbcd")
    // {0xF1F0, 0xC100, INSTRUCTION::Abcd::create},       //(0b1111000111110000, 0b1100000100000000, "Abcd")
    // {0xFEC0, 0xE0C0, INSTRUCTION::Asd::create},        //(0b1111111011000000, 0b1110000011000000, "Asd")
    // {0xFEC0, 0xE2C0, INSTRUCTION::Lsd::create},        //(0b1111111011000000, 0b1110001011000000, "Lsd")
    // {0xFEC0, 0xE4C0, INSTRUCTION::Roxd::create},       //(0b1111111011000000, 0b1110010011000000, "Roxd")
    // {0xFEC0, 0xE6C0, INSTRUCTION::Rod::create},        //(0b1111111011000000, 0b1110011011000000, "Rod")
    {0xFF00, 0x0000, INSTRUCTION::Ori::create},        //(0b1111111100000000, 0b0000000000000000, "Ori")
    {0xFF00, 0x0200, INSTRUCTION::Andi::create},       //(0b1111111100000000, 0b0000001000000000, "Andi")
    {0xFF00, 0x0400, INSTRUCTION::Subi::create},       //(0b1111111100000000, 0b0000010000000000, "Subi")
    {0xFF00, 0x0600, INSTRUCTION::Addi::create},       //(0b1111111100000000, 0b0000011000000000, "Addi")
    {0xFF00, 0x0A00, INSTRUCTION::Eori::create},       //(0b1111111100000000, 0b0000101000000000, "Eori")
    {0xFF00, 0x0C00, INSTRUCTION::Cmpi::create},       //(0b1111111100000000, 0b0000110000000000, "Cmpi")
    // {0xF138, 0x0108, INSTRUCTION::Movep::create},      //(0b1111000100111000, 0b0000000100001000, "Movep")
    // {0xFF00, 0x4000, INSTRUCTION::Negx::create},       //(0b1111111100000000, 0b0100000000000000, "Negx")
    {0xFF00, 0x4200, INSTRUCTION::Clr::create},        //(0b1111111100000000, 0b0100001000000000, "Clr")
    {0xFF00, 0x4400, INSTRUCTION::Neg::create},        //(0b1111111100000000, 0b0100010000000000, "Neg")
    // {0xFF00, 0x4600, INSTRUCTION::Not::create},        //(0b1111111100000000, 0b0100011000000000, "Not")
    {0xFF00, 0x4A00, INSTRUCTION::Tst::create},        //(0b1111111100000000, 0b0100101000000000, "Tst")
    // {0xFB80, 0x4880, INSTRUCTION::Movem::create},      //(0b1111101110000000, 0b0100100010000000, "Movem")
    {0xFF00, 0x6000, INSTRUCTION::Bcc::create},        //(0b1111111100000000, 0b0110000000000000, "Bra")
    {0xFF00, 0x6100, INSTRUCTION::Bcc::create},        //(0b1111111100000000, 0b0110000100000000, "Bsr")
    {0xF0C0, 0xB0C0, INSTRUCTION::Cmpa::create},       //(0b1111000011000000, 0b1011000011000000, "Cmpa")
    // {0xF1C0, 0x0100, INSTRUCTION::Btst::create},       //(0b1111000111000000, 0b0000000100000000, "Btst")
    // {0xF1C0, 0x0140, INSTRUCTION::Bchg::create},       //(0b1111000111000000, 0b0000000101000000, "Bchg")
    // {0xF1C0, 0x0180, INSTRUCTION::Bclr::create},       //(0b1111000111000000, 0b0000000110000000, "Bclr")
    // {0xF1C0, 0x01C0, INSTRUCTION::Bset::create},       //(0b1111000111000000, 0b0000000111000000, "Bset")
    {0xF1C0, 0x3040, INSTRUCTION::Move::create},     //(0b1111000111000000, 0b0011000001000000, "Moveaw")
    {0xF1C0, 0x2040, INSTRUCTION::Move::create},     //(0b1111000111000000, 0b0010000001000000, "Moveal")
    {0xF1C0, 0x41C0, INSTRUCTION::Lea::create},        //(0b1111000111000000, 0b0100000111000000, "Lea")
    // {0xF1C0, 0x4180, INSTRUCTION::Chk::create},        //(0b1111000111000000, 0b0100000110000000, "Chk")
    {0xF1C0, 0x80C0, INSTRUCTION::Div::create},       //(0b1111000111000000, 0b1000000011000000, "Divu")
    {0xF1C0, 0x81C0, INSTRUCTION::Div::create},       //(0b1111000111000000, 0b1000000111000000, "Divs")
    {0xF0C0, 0x90C0, INSTRUCTION::Suba::create},       //(0b1111000011000000, 0b1001000011000000, "Suba")
    {0xF1C0, 0xC0C0, INSTRUCTION::Mul::create},       //(0b1111000111000000, 0b1100000011000000, "Mulu")
    {0xF1C0, 0xC1C0, INSTRUCTION::Mul::create},       //(0b1111000111000000, 0b1100000111000000, "Muls")
    // {0xF130, 0xC100, INSTRUCTION::Exg::create},        //(0b1111000100110000, 0b1100000100000000, "Exg")
    {0xF000, 0xC000, INSTRUCTION::And::create},        //(0b1111000000000000, 0b1100000000000000, "And")
    {0xF0C0, 0xD0C0, INSTRUCTION::Adda::create},       //(0b1111000011000000, 0b1101000011000000, "Adda")
    {0xF130, 0xD100, INSTRUCTION::Addx::create},       //(0b1111000100110000, 0b1101000100000000, "Addx")
    {0xF0C0, 0x50C0, INSTRUCTION::Scc::create},        //(0b1111000011000000, 0b0101000011000000, "Scc")
    {0xF130, 0x9100, INSTRUCTION::Subx::create},       //(0b1111000100110000, 0b1001000100000000, "Subx")
    // {0xF138, 0xB108, INSTRUCTION::Cmpm::create},       //(0b1111000100111000, 0b1011000100001000, "Cmpm")
    // {0xF018, 0xE000, INSTRUCTION::Asd::create},        //(0b1111000000011000, 0b1110000000000000, "Asd")
    // {0xF018, 0xE008, INSTRUCTION::Lsd::create},        //(0b1111000000011000, 0b1110000000001000, "Lsd")
    // {0xF018, 0xE010, INSTRUCTION::Roxd::create},       //(0b1111000000011000, 0b1110000000010000, "Roxd")
    // {0xF018, 0xE018, INSTRUCTION::Rod::create},        //(0b1111000000011000, 0b1110000000011000, "Rod")
    {0xF100, 0x5000, INSTRUCTION::Addq::create},       //(0b1111000100000000, 0b0101000000000000, "Addq")
    {0xF100, 0x5100, INSTRUCTION::Subq::create},       //(0b1111000100000000, 0b0101000100000000, "Subq")
    {0xF100, 0x7000, INSTRUCTION::Moveq::create},      //(0b1111000100000000, 0b0111000000000000, "Moveq")
    {0xF100, 0xB100, INSTRUCTION::Eor::create},        //(0b1111000100000000, 0b1011000100000000, "Eor")
    {0xF100, 0xB000, INSTRUCTION::Cmp::create},        //(0b1111000100000000, 0b1011000000000000, "Cmp")
    {0xF000, 0x3000, INSTRUCTION::Move::create},      //(0b1111000000000000, 0b0011000000000000, "Movew")
    {0xF000, 0x2000, INSTRUCTION::Move::create},      //(0b1111000000000000, 0b0010000000000000, "Movel")
    {0xF000, 0x1000, INSTRUCTION::Move::create},      //(0b1111000000000000, 0b0001000000000000, "Moveb")
    {0xF000, 0x6000, INSTRUCTION::Bcc::create},        //(0b1111000000000000, 0b0110000000000000, "Bcc")
    {0xF000, 0x8000, INSTRUCTION::Or::create},         //(0b1111000000000000, 0b1000000000000000, "Or")
    {0xF000, 0x9000, INSTRUCTION::Sub::create},        //(0b1111000000000000, 0b1001000000000000, "Sub")
    {0xF000, 0xD000, INSTRUCTION::Add::create},        //(0b1111000000000000, 0b1101000000000000, "Add")
};

void InstructionDecoder::generateOpcodeTable(){
    this->opcode_table.clear();
    this->opcode_table.reserve(0x10000);

    for(uint32_t opcode = 0; opcode < 0x10000; opcode++){
        std::shared_ptr<M68K::INSTRUCTION::Instruction> instruction = INSTRUCTION::Illegal::create(opcode);
        for(auto& opcode_mask : opcode_mask_table){
            if((opcode & opcode_mask.mask) == opcode_mask.value){
                auto instruction_candidate = opcode_mask.create_function(opcode);
                if(instruction_candidate.get()->is_valid){
                    instruction = instruction_candidate;
                }
                break;
            }
        }
        this->opcode_table.emplace_back(instruction);
    }
}

InstructionDecoder::InstructionDecoder(){
    this->generateOpcodeTable();
}

INSTRUCTION::Instruction* InstructionDecoder::Decode(uint16_t opcode){
    return opcode_table[opcode].get();
}
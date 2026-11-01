#include <catch2/catch_test_macros.hpp>

#include "m68k.hpp"
#include "memory.hpp"
#include "helpers.hpp"
#include "test_helpers.hpp"

using namespace M68K;

TEST_CASE("Validity: Dn,<ea> forms reject register destinations", "[instruction][validity]") {
    CHECK(INSTRUCTION::And::create(0xC100).get()->is_valid == false); // abcd D0, D0
    CHECK(INSTRUCTION::And::create(0xC108).get()->is_valid == false); // abcd -(A0), -(A0)
    CHECK(INSTRUCTION::And::create(0xC140).get()->is_valid == false); // exg D0, D0
    CHECK(INSTRUCTION::And::create(0xC148).get()->is_valid == false); // exg A0, A0
    CHECK(INSTRUCTION::And::create(0xC188).get()->is_valid == false); // exg D0, A0
    CHECK(INSTRUCTION::And::create(0xC150).get()->is_valid == true); // and.w D0, (A0)
    CHECK(INSTRUCTION::Or::create(0x8100).get()->is_valid == false); // sbcd D0, D0
    CHECK(INSTRUCTION::Or::create(0x8108).get()->is_valid == false); // sbcd -(A0), -(A0)
    CHECK(INSTRUCTION::Or::create(0x8150).get()->is_valid == true); // or.w D0, (A0)
    CHECK(INSTRUCTION::Add::create(0xD300).get()->is_valid == false); // addx.b D0, D1
    CHECK(INSTRUCTION::Sub::create(0x9300).get()->is_valid == false); // subx.b D0, D1
}

TEST_CASE("Validity: <ea>,Dn forms reject address register source", "[instruction][validity]") {
    CHECK(INSTRUCTION::And::create(0xC048).get()->is_valid == false); // and.w A0, D0
    CHECK(INSTRUCTION::Or::create(0x8048).get()->is_valid == false); // or.w A0, D0
    CHECK(INSTRUCTION::Mul::create(0xC0C8).get()->is_valid == false); // mulu A0, D0
    CHECK(INSTRUCTION::Div::create(0x80C8).get()->is_valid == false); // divu A0, D0
    CHECK(INSTRUCTION::And::create(0xC040).get()->is_valid == true); // and.w D0, D0
}

TEST_CASE("Validity: data alterable destinations reject An", "[instruction][validity]") {
    CHECK(INSTRUCTION::Eor::create(0xB108).get()->is_valid == false); // cmpm.b (A0)+, (A0)+
    CHECK(INSTRUCTION::Eor::create(0xB148).get()->is_valid == false); // cmpm.w (A0)+, (A0)+
    CHECK(INSTRUCTION::Eor::create(0xB101).get()->is_valid == true); // eor.b D0, D1
    CHECK(INSTRUCTION::Andi::create(0x0248).get()->is_valid == false); // andi.w #, A0
    CHECK(INSTRUCTION::Ori::create(0x0048).get()->is_valid == false); // ori.w #, A0
    CHECK(INSTRUCTION::Eori::create(0x0A48).get()->is_valid == false); // eori.w #, A0
    CHECK(INSTRUCTION::Addi::create(0x0648).get()->is_valid == false); // addi.w #, A0
    CHECK(INSTRUCTION::Subi::create(0x0448).get()->is_valid == false); // subi.w #, A0
    CHECK(INSTRUCTION::Cmpi::create(0x0C48).get()->is_valid == false); // cmpi.w #, A0
    CHECK(INSTRUCTION::Neg::create(0x4448).get()->is_valid == false); // neg.w A0
    CHECK(INSTRUCTION::Clr::create(0x4248).get()->is_valid == false); // clr.w A0
    CHECK(INSTRUCTION::Tst::create(0x4A48).get()->is_valid == false); // tst.w A0
    CHECK(INSTRUCTION::Andi::create(0x0240).get()->is_valid == true); // andi.w #, D0
    CHECK(INSTRUCTION::Tst::create(0x4A40).get()->is_valid == true); // tst.w D0
}

TEST_CASE("Validity: quick and move forms", "[instruction][validity]") {
    CHECK(INSTRUCTION::Addq::create(0x5208).get()->is_valid == false); // addq.b #1, A0
    CHECK(INSTRUCTION::Subq::create(0x5308).get()->is_valid == false); // subq.b #1, A0
    CHECK(INSTRUCTION::Addq::create(0x5248).get()->is_valid == true); // addq.w #1, A0
    CHECK(INSTRUCTION::Subq::create(0x5348).get()->is_valid == true); // subq.w #1, A0
    CHECK(INSTRUCTION::Move::create(0x3200).get()->is_valid == true); // move.w D0, D1
    CHECK(INSTRUCTION::Move::create(0x3240).get()->is_valid == true); // movea.w D0, A1
    CHECK(INSTRUCTION::Move::create(0x1240).get()->is_valid == false); // move.b D0, A1
}

TEST_CASE("Validity: bit shift and bit manipulation", "[instruction][validity]") {
    CHECK(INSTRUCTION::BitShift::create(0xE0C0).get()->is_valid == false); // asr.w D0 (memory form)
    CHECK(INSTRUCTION::BitShift::create(0xE0C8).get()->is_valid == false); // asr.w A0 (memory form)
    CHECK(INSTRUCTION::BitShift::create(0xE0D0).get()->is_valid == true); // asr.w (A0)
    CHECK(INSTRUCTION::BitManip::create(0x017A).get()->is_valid == false); // bchg D0, d16(PC)
    CHECK(INSTRUCTION::BitManip::create(0x083C).get()->is_valid == false); // btst #, #imm
    CHECK(INSTRUCTION::BitManip::create(0x0108).get()->is_valid == false); // movep
    CHECK(INSTRUCTION::BitManip::create(0x0101).get()->is_valid == true); // btst D0, D1
}

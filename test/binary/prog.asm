.org 0x0000
jmp _start

sub:
  move.l #10000, %d0
  rts
_start:
  move.l #10000, %d1
  jsr sub
  add.l %d0,%d1

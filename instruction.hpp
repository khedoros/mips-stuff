#pragma once

#include<cstdint>

//     OP   RS   RT   RD   RE  FUNC   R-Type
//     6     5    5    5    5    6

//     OP   RS   RT  IMMED            I-Type
//     6     5    5   16

//     OP  TARGET                     J-Type
//     6     26

/*
  31..26 |25..21|20..16|15..11|10..6 |  5..0  |
   6bit  | 5bit | 5bit | 5bit | 5bit |  6bit  |
  -------+------+------+------+------+--------+------------
  000000 | N/A  | rt   | rd   | imm5 | 0000xx | shift-imm
  000000 | rs   | rt   | rd   | N/A  | 0001xx | shift-reg
  000000 | rs   | N/A  | N/A  | N/A  | 001000 | jr
  000000 | rs   | N/A  | rd   | N/A  | 001001 | jalr
  000000 | <-----comment20bit------> | 00110x | sys/brk
  000000 | N/A  | N/A  | rd   | N/A  | 0100x0 | mfhi/mflo
  000000 | rs   | N/A  | N/A  | N/A  | 0100x1 | mthi/mtlo
  000000 | rs   | rt   | N/A  | N/A  | 0110xx | mul/div
  000000 | rs   | rt   | rd   | N/A  | 10xxxx | alu-reg

  000001 | rs   | 00000| <--immediate16bit--> | bltz
  000001 | rs   | 00001| <--immediate16bit--> | bgez
  000001 | rs   | 10000| <--immediate16bit--> | bltzal
  000001 | rs   | 10001| <--immediate16bit--> | bgezal
  000001 | rs   | xxxx0| <--immediate16bit--> | bltz  ;\undocumented dupes
  000001 | rs   | xxxx1| <--immediate16bit--> | bgez  ;/(when bit17-19=nonzero)

  00001x | <---------immediate26bit---------> | j/jal
  00010x | rs   | rt   | <--immediate16bit--> | beq/bne
  00011x | rs   | N/A  | <--immediate16bit--> | blez/bgtz
  001xxx | rs   | rt   | <--immediate16bit--> | alu-imm
  001111 | N/A  | rt   | <--immediate16bit--> | lui-imm
  100xxx | rs   | rt   | <--immediate16bit--> | load rt,[rs+imm]
  101xxx | rs   | rt   | <--immediate16bit--> | store rt,[rs+imm]
  x1xxxx | <------coprocessor specific------> | coprocessor (see below)
*/

struct inst_t {
    uint32_t opcode; // full 32-bit operation fetched

    unsigned op:6; // 0 for r-format, 0x10 and 0x11 for j-format, 
    unsigned func:6; // only used for that purpose in r-format
    unsigned rs:5; // r-format op 1, i-format offset reg
    unsigned rt:5; // i-format load src/dest, r-format op 2
    unsigned rd:5; // r-format dest
    unsigned imm5:5; // R-format immediate
    unsigned imm16:16; // I-format
    unsigned target:26; // J-format
    static uint32_t getOp(uint32_t inst) { return inst>>26; }
    static uint32_t getFunc(uint32_t inst) { return inst & 0x3f; }
    static uint32_t getRs(uint32_t inst) { return (inst >> 21) & 0x1f; }
    static uint32_t getRt(uint32_t inst) { return (inst >> 16) & 0x1f; }
    static uint32_t getRd(uint32_t inst) { return (inst >> 11) & 0x1f; }
    static uint32_t getImm5(uint32_t inst) { return (inst >>  6) & 0x1f; }
    static int16_t getImm16(uint32_t inst) { return inst & 0xffff; }
    static uint32_t getTarget(uint32_t inst) { return 4*(inst & 0x3ff'ffff); }
};
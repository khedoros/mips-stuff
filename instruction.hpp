#pragma once

#include<cstdint>

struct inst_t {
    unsigned op:6;
    unsigned func:6;
    unsigned rs:5;
    unsigned rt:5;
    unsigned rd:5;
    unsigned re:5;
    unsigned imm5:5;
    unsigned imm16:16;
    unsigned target:26;
    static uint32_t getOp(uint32_t inst) { return inst>>26; }
    static uint32_t getRs(uint32_t inst) { return (inst >> 21) & 0x1f; }
    static uint32_t getRt(uint32_t inst) { return (inst >> 16) & 0x1f; }
    static uint32_t getRd(uint32_t inst) { return (inst >> 11) & 0x1f; }
    static uint32_t getRe(uint32_t inst) { return (inst >>  6) & 0x1f; }
    static uint32_t getFunc(uint32_t inst) { return inst & 0x3f; }
    static uint32_t getImm5(uint32_t inst) { return (inst >>  6) & 0x1f; }
    static int16_t getImm16(uint32_t inst) { return inst & 0xffff; }
    static uint32_t getTarget(uint32_t inst) { return 4*(inst & 0x3ff'ffff); }
};
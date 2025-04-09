#include "instruction.hpp"

namespace inst {
    uint32_t getOp(uint32_t inst) { return inst >> 26; }
    uint32_t getFunc(uint32_t inst) { return inst & 0x3f; }
    uint32_t getRs(uint32_t inst) { return (inst >> 21) & 0x1f; }
    uint32_t getRt(uint32_t inst) { return (inst >> 16) & 0x1f; }
    uint32_t getRd(uint32_t inst) { return (inst >> 11) & 0x1f; }
    uint32_t getImm5(uint32_t inst) { return (inst >>  6) & 0x1f; }
    int16_t getImm16(uint32_t inst) { return inst & 0xffff; }
    uint32_t getTarget(uint32_t inst) { return 4*(inst & 0x3ff'ffff); }
}
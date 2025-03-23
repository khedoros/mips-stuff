#pragma once

#include<cstdint>

class cpu {
private:
    typedef uint64_t cycleCount;
    cycleCount cycle;
    uint32_t reg[32];
    uint32_t pc;
    uint32_t hi,lo;

    cycleCount executeBranch(uint32_t inst);
    cycleCount executeAlu(uint32_t inst);

public:
    cpu();
    cycleCount execute(uint32_t inst);
};
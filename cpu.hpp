#pragma once

#include<cstdint>
#include<array>
#include<memory>

#include "instruction.hpp"
#include "memmap.hpp"

class cpu {
private:
    std::unique_ptr<memmap> mem;
    typedef uint64_t cycleCount;
    cycleCount cycle;
    uint32_t reg[32];
    uint32_t pc;
    uint32_t hi,lo;

    std::size_t pipelineStep;
    static const std::size_t pipelineLength = 5;
    enum pipelineStep {
        fetch,     // fetch from RAM
        decode,    // instruction decode+register read
        execute,   // execute operation or calculate address
        memory,    // access memory operand
        writeback  // write result back to register
    };
    std::array<inst_t, pipelineLength> pipeline;
    cycleCount exec(uint32_t inst);
    cycleCount execBranch(uint32_t inst);
    cycleCount execAlu(uint32_t inst);
    cycleCount fetchOp(inst_t& i);
    cycleCount decodeOp(inst_t& i);
    cycleCount executeOp(inst_t& i);
    cycleCount memoryOp(inst_t& i);
    cycleCount writebackOp(inst_t& i);

public:
    cpu(std::unique_ptr<memmap>& memoryMap);
    cycleCount runOne();
};
#include<algorithm>
#include<iostream>
#include<fstream>
#include<vector>
#include<string>
#include<cstdint>

#include "cpu.hpp"
#include "memmap.hpp"
#include "instruction.hpp"
#include "disassembly.hpp"

cpu::cpu(std::unique_ptr<memmap>& memoryMap) : cycle{0}, pc{0xbfc00000}, mem(std::move(memoryMap)) {
    
}

cpu::cycleCount cpu::runOne() {

    cycleCount fcc = fetchOp(pipeline[(pipelineStep + fetch) % pipelineLength]);
    
    cycleCount dcc = decodeOp(pipeline[(pipelineStep + decode) % pipelineLength]);

    cycleCount ecc = executeOp(pipeline[(pipelineStep + execute) % pipelineLength]);

    cycleCount mcc = memoryOp(pipeline[(pipelineStep + memory) % pipelineLength]);

    cycleCount wcc = writebackOp(pipeline[(pipelineStep + writeback) % pipelineLength]);

    pipelineStep++;
    pipelineStep %= 5;

    return std::max({fcc,dcc,ecc,mcc,wcc});
}

cpu::cycleCount cpu::fetchOp(inst_t& inst) {
    inst.opcode = mem->read32(pc);
    inst.valid = false;
    inst.regWB = false;
    inst.jump = false;
    inst.memOpType = inst_t::none;
    return 1;
}

cpu::cycleCount cpu::decodeOp(inst_t& inst) {
    inst.setOp();
    switch(inst.op) {
    case 0: // reg-reg instructions
        inst.setFunc();
        inst.setRs();
        inst.setRd();
        inst.setRt();
        inst.operand1 = reg[inst.rs];
        inst.operand2 = reg[inst.rt];
        inst.outputReg = inst.rd;
        inst.regWB = true;
        break;
    case 1: // branch ops
        inst.setRs();
        inst.setRt(); // contains branch type
        inst.setImm16();
        inst.jump = true;
        break;
    case 2: case 3: // j, jal
        inst.setTarget();
        inst.jump = true;
        break;
    case 4: case 5: // beq, bne
        inst.setRs();
        inst.setRt();
        inst.setImm16();
        inst.jump = true;
        break;
    case 6: case 7: // blez, bgtz
        inst.setRs();
        inst.setImm16();
        inst.jump = true;
        break;
    case 8: case 9: case 0xa: case 0xb: case 0xc: case 0xd: case 0xe: // addi, addiu, slti, sltiu, andi, ori, xori
        inst.setImm16();
        inst.setRt();
        inst.setRs();
        inst.regWB = true;
        inst.outputReg = inst.rt;
        break;
    case 0xf: // lui
        inst.setImm16();
        inst.setRt();
        inst.regWB = true;
        inst.outputReg = inst.rt;
        break;
    case 0x10: case 0x11: case 0x12: case 0x13: // cop0, cop1, cop2, cop3
    case 0x20: case 0x21: case 0x22: case 0x23: case 0x24: case 0x25: case 0x26: // lb, lh, lwl, lw, lbu, lhu, lwr
    case 0x28: case 0x29: case 0x2a: case 0x2b: case 0x2e: // sb, sh, swl, sw, swr
    case 0x30: case 0x31: case 0x32: case 0x33: // lwc0, lwc1, lwc2, lwc3
    case 0x38: case 0x39: case 0x3a: case 0x3b: // swc0, swc1, swc2, swc3

    default:
        std::cerr<<"Unhandled Op\n";
        return 0;
    }
    inst.valid = true;
    return 1;
}

cpu::cycleCount cpu::exec(const inst_t& inst) {
    std::cout << ::decode(inst.opcode) << '\n';
    switch(inst.op) {
        case 0: return execAlu(inst); // ALU operations
        case 1: return execBranch(inst); // Branch operations
        case 2: // j
            return 0;
        case 3: // jal
            return 0;
            // return opcode[oper] + sp + std::to_string(inst_t::getTarget(inst));
        case 4: // beq
            return 0;
            // return opcode[inst_t::getOp(inst)] + " meep";
        case 5: // bne
            return 0;
            // return opcode[inst_t::getOp(inst)] + " meep";
        case 6: // blez
            return 0;
            // return opcode[inst_t::getOp(inst)] + " meep";
        case 7: // bgtz
            return 0;
            // return opcode[inst_t::getOp(inst)] + " meep";
        case 8: // addi
            return 0;
        case 9: // addiu
            return 0;
        case 0xa: // slti
            return 0;
        case 0xb: // sltiu
            return 0;
        case 0xc: // andi
            return 0;
        case 0xd: // ori
            return 0;
        case 0xe: // xori
            return 0;
            // return opcode[oper] + sp + regName[inst_t::getRt(inst)] + csp + regName[inst_t::getRs(inst)] + csp + std::to_string(inst_t::getImm16(inst));
        case 0xf: // lui
            return 0;
            // return opcode[oper] + sp + regName[inst_t::getRt(inst)] + csp + std::to_string(inst_t::getImm16(inst));
        case 0x10: // cop0
            return 0;
            // return opcode[inst_t::getOp(inst)] + " meep";
        case 0x11: // cop1
            return 0;
            //return opcode[inst_t::getOp(inst)] + " meep";
        case 0x12: // cop2
            return 0;
            // return opcode[inst_t::getOp(inst)] + " meep";
        case 0x13: // cop3
            return 0;
            // return opcode[inst_t::getOp(inst)] + " meep";
        case 0x20: // lb
            return 0;
        case 0x21: // lh
            return 0;
        case 0x22: // lwl
            return 0;
        case 0x23: // lw
            return 0;
        case 0x24: // lbu
            return 0;
        case 0x25: // lhu
            return 0;
        case 0x26: // lwr
            return 0;
        case 0x28: // sb
            return 0;
        case 0x29: // sh
            return 0;
        case 0x2a: // swl
            return 0;
        case 0x2b: // sw
            return 0;
        case 0x2e: // swr
            return 0;
            // return opcode[oper] + sp + regName[inst_t::getRt(inst)] + csp + std::to_string(inst_t::getImm16(inst)) + op + regName[inst_t::getRs(inst)] + cp;
        case 0x30: // lwc0
            return 0;
            // return opcode[inst_t::getOp(inst)] + " meep";
        case 0x31: // lwc1
            return 0;
            // return opcode[inst_t::getOp(inst)] + " meep";
        case 0x32: // lwc2
            return 0;
            // return opcode[inst_t::getOp(inst)] + " meep";
        case 0x33: // lwc3
            return 0;
            // return opcode[inst_t::getOp(inst)] + " meep";
        case 0x38: // swc0
            return 0;
            // return opcode[inst_t::getOp(inst)] + " meep";
        case 0x39: // swc1
            return 0;
            // return opcode[inst_t::getOp(inst)] + " meep";
        case 0x3a: // swc2
            return 0;
            // return opcode[inst_t::getOp(inst)] + " meep";
        case 0x3b: // swc3
            return 0;
            // return opcode[inst_t::getOp(inst)] + " meep";
        default: 
            return 0;
            // Unrecognized/invalid
    }
}

cpu::cycleCount cpu::execAlu(const inst_t& inst) {
    switch(inst.func) {
        case 0: // sll
            return 0;
        case 2: // srl
            return 0;
        case 3: // sra
            return 0;
        case 4: // sllv
            return 0;
        case 6: // srlv
            return 0;
        case 7: // srav
            return 0;
        case 8: // jr
            return 0;
            // return func[inst_t::getFunc(inst)] + sp + regName[inst_t::getRs(inst)];
        case 9: // jalr
            return 0;
            // return func[inst_t::getFunc(inst)] + sp + regName[inst_t::getRd(inst)] + csp + regName[inst_t::getRs(inst)];
        case 0xc: // syscall
            return 0;
        case 0xd: // break
            return 0;
            // return func[inst_t::getFunc(inst)] + sp + std::to_string(inst>>6); // top 6 bits are 0, next 20 bits are a "comment" value
        case 0x10: // mfhi
            return 0;
        case 0x11: // mthi
            return 0;
        case 0x12: // mflo
            return 0;
        case 0x13: // mtlo
            return 0;
        case 0x18: // mult
            return 0;
        case 0x19: // multu
            return 0;
        case 0x1a: // div
            return 0;
        case 0x1b: // divu
            return 0;
        case 0x20: // add
            return 0;
        case 0x21: // addu
            return 0;
        case 0x22: // sub
            return 0;
        case 0x23: // subu
            return 0;
        case 0x24: // and
            return 0;
        case 0x25: // or
            return 0;
        case 0x26: // xor
            return 0;
        case 0x27: // nor
            return 0;
        case 0x2a: // slt
            return 0;
        case 0x2b: // sltu
            return 0;
        default:
            return 0;
            // unrecognized/invalid
    }
}

cpu::cycleCount cpu::execBranch(const inst_t& inst) {
    // branch type: inst_t::getRt(inst)
    // offset register: inst_t::getRs(inst)
    // 16-bit immediate: inst_t::getImm16(inst);
    switch(inst.rt) {
        case 0x10:
            return 0;
        case 0x11:
            return 0;
        default:
            if(inst.rt & 1) { // bgezal
                return 0;
            }
            else { // bltzal
                return 0;
            }
    }
}

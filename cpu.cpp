#include<iostream>
#include<fstream>
#include<vector>
#include<string>
#include<cstdint>

#include "cpu.hpp"
#include "instruction.hpp"
#include "disassembly.hpp"

cpu::cycleCount cpu::executeBranch(uint32_t inst) {
    // branch type: inst_t::getRt(inst)
    // offset register: inst_t::getRs(inst)
    // 16-bit immediate: inst_t::getImm16(inst);
    return 0;
}

cpu::cycleCount cpu::executeAlu(uint32_t inst) {
    switch(inst_t::getFunc(inst)) {
        case 0: case 2: case 3: // SLL SRL SRA
            if(inst == 0) return 0; // "nop";
            // function: inst_t::getFunc(inst)
            // destination: inst_t::getRd(inst)
            // register operator: inst_t::getRt(inst)
            // rotation constant: inst_t::getImm5(inst)
            return 0;
        case 4: case 6: case 7: // SLLV SRLV SRAV
            return 0;
            // function: inst_t::getFunc(inst)
            // destination: inst_t::getRd(inst)
            // register operator: inst_t::getRt(inst)
            // rotation register: inst_t::getRs(inst);
        case 8: // JR
            return 0;
            // return func[inst_t::getFunc(inst)] + sp + regName[inst_t::getRs(inst)];
        case 9: // JALR
            return 0;
            // return func[inst_t::getFunc(inst)] + sp + regName[inst_t::getRd(inst)] + csp + regName[inst_t::getRs(inst)];
        case 12: case 13:
            return 0;
            // return func[inst_t::getFunc(inst)] + sp + std::to_string(inst>>6); // top 6 bits are 0, next 20 bits are a "comment" value
        case 16: case 18:
            return 0;
            // return func[inst_t::getFunc(inst)] + sp + regName[inst_t::getRd(inst)];
        case 17: case 19:
            return 0;
            // return func[inst_t::getFunc(inst)] + sp + regName[inst_t::getRs(inst)];
        case 24: case 25: case 26: case 27:
            return 0;
            // return func[inst_t::getFunc(inst)] + sp + regName[inst_t::getRs(inst)] + csp + regName[inst_t::getRt(inst)];
        case 32: case 33: case 34: case 35: case 36: case 37: case 38: case 39: case 42: case 43:
            return 0;
            // return func[inst_t::getFunc(inst)] + sp + regName[inst_t::getRd(inst)] + csp + regName[inst_t::getRs(inst)] + csp + regName[inst_t::getRt(inst)];
        default:
            return 0;
            // unrecognized/invalid
    }
}

cpu::cycleCount cpu::execute(uint32_t inst) {
    std::cout << ::decode(inst) << '\n';
    auto oper = inst_t::getOp(inst);
    switch(oper) {
        case 0: return executeAlu(inst); // ALU operations
        case 1: return executeBranch(inst); // Branch operations
        case 2: case 3: // j/jal
            return 0;
            // return opcode[oper] + sp + std::to_string(inst_t::getTarget(inst));
        case 4:
            return 0;
            // return opcode[inst_t::getOp(inst)] + " meep";
        case 5:
            return 0;
            // return opcode[inst_t::getOp(inst)] + " meep";
        case 6:
            return 0;
            // return opcode[inst_t::getOp(inst)] + " meep";
        case 7:
            return 0;
            // return opcode[inst_t::getOp(inst)] + " meep";
        case 8: case 9: case 10: case 11: case 12: case 13: case 14: // I-Type ALU ops
            return 0;
            // return opcode[oper] + sp + regName[inst_t::getRt(inst)] + csp + regName[inst_t::getRs(inst)] + csp + std::to_string(inst_t::getImm16(inst));
        case 15: // lui
            return 0;
            // return opcode[oper] + sp + regName[inst_t::getRt(inst)] + csp + std::to_string(inst_t::getImm16(inst));
        case 16:
            return 0;
            // return opcode[inst_t::getOp(inst)] + " meep";
        case 17:
            return 0;
            //return opcode[inst_t::getOp(inst)] + " meep";
        case 18:
            return 0;
            // return opcode[inst_t::getOp(inst)] + " meep";
        case 19:
            return 0;
            // return opcode[inst_t::getOp(inst)] + " meep";
        case 32: case 33: case 34: case 35: case 36: case 37: case 38: case 40: case 41: case 42: case 43: case 46: // load/store ops
            return 0;
            // return opcode[oper] + sp + regName[inst_t::getRt(inst)] + csp + std::to_string(inst_t::getImm16(inst)) + op + regName[inst_t::getRs(inst)] + cp;
        case 48:
            return 0;
            // return opcode[inst_t::getOp(inst)] + " meep";
        case 49:
            return 0;
            // return opcode[inst_t::getOp(inst)] + " meep";
        case 50:
            return 0;
            // return opcode[inst_t::getOp(inst)] + " meep";
        case 51:
            return 0;
            // return opcode[inst_t::getOp(inst)] + " meep";
        case 56:
            return 0;
            // return opcode[inst_t::getOp(inst)] + " meep";
        case 57:
            return 0;
            // return opcode[inst_t::getOp(inst)] + " meep";
        case 58:
            return 0;
            // return opcode[inst_t::getOp(inst)] + " meep";
        case 59:
            return 0;
            // return opcode[inst_t::getOp(inst)] + " meep";
        default: 
            return 0;
            // Unrecognized/invalid
    }
}

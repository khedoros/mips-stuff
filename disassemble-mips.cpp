#include<iostream>
#include<fstream>
#include<vector>
#include<string>
#include<cstdint>

enum format_t {
    reg,
    imm,
    jmp
};

using str = std::string;
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

static const std::string instFormat[] {
//     OP   RS   RT   RD   RE  FUNC   R-Type
//     6     5    5    5    5    6
    {"%02x %02x %02x %02x %02x %02x\n"},
//     OP   RS   RT  IMMED            I-Type
//     6     5    5   16
    {"%02x %02x %02x %04x\n"},
//     OP  TARGET                     J-Type
//     6     26
    {"%02x %07x\n"}
};

static const std::string regName[] {
    "$zero", "$at", "$v0", "$v1",
    "$a0", "$a1", "$a2", "$a3",
    "$t0", "$t1", "$t2", "$t3",
    "$t4", "$t5", "$t6", "$t7",
    "$s0", "$s1", "$s2", "$s3",
    "$s4", "$s5", "$s6", "$s7",
    "$t8", "$t9", "$k0", "$k1",
    "$gp", "$sp", "$fp", "$ra"
};

static const std::string opcode[] {
    "special", "bCONDz", "j",    "jal",   "beq",  "bne", "blez", "bgtz",
    "addi",    "addiu",  "slti", "sltiu", "andi", "ori", "xori", "lui",
    "cop0",    "cop1",   "cop2", "cop3",  "n/a",  "n/a", "n/a",  "n/a",
    "n/a",     "n/a",    "n/a",  "n/a",   "n/a",  "n/a", "n/a",  "n/a",
    "lb",      "lh",     "lwl",  "lw",    "lbu",  "lhu", "lwr",  "n/a",
    "sb",      "sh",     "swl",  "sw",    "n/a",  "n/a", "swr",  "n/a",
    "lwc0",    "lwc1",   "lwc2", "lwc3",  "n/a",  "n/a", "n/a",  "n/a",
    "swc0",    "swc1",   "swc2", "swc3",  "n/a",  "n/a", "n/a",  "n/a"
};

// SPECIAL table
static const std::string func[] {
    "sll",  "n/a",   "srl",  "sra",  "sllv",    "n/a",   "srlv", "srav",
    "jr",   "jalr",  "n/a",  "n/a",  "syscall", "break", "n/a",  "n/a",
    "mfhi", "mthi",  "mflo", "mtlo", "n/a",     "n/a",   "n/a",  "n/a",
    "mult", "multu", "div",  "divu", "n/a",     "n/a",   "n/a",  "n/a",
    "add",  "addu",  "sub",  "subu", "and",     "or",    "xor",  "nor",
    "n/a",  "n/a",   "slt",  "sltu", "n/a",     "n/a",   "n/a",  "n/a",
    "n/a",  "n/a",   "n/a",  "n/a",  "n/a",     "n/a",   "n/a",  "n/a"
    "n/a",  "n/a",   "n/a",  "n/a",  "n/a",     "n/a",   "n/a",  "n/a"
};

// BcondZ table
static const std::string cond[] {
    "bltz",   "bgez",   "bltz", "bgez", "bltz", "bgez", "bltz", "bgez", 
    "bltz",   "bgez",   "bltz", "bgez", "bltz", "bgez", "bltz", "bgez", 
    "bltzal", "bgezal", "bltz", "bgez", "bltz", "bgez", "bltz", "bgez", 
    "bltz",   "bgez",   "bltz", "bgez", "bltz", "bgez", "bltz", "bgez"
};

static const str sp = " ";
static const str csp = ", ";
static const str op = "(";
static const str cp = ")";

std::string decodeBranch(uint32_t inst) {
    return cond[inst_t::getRt(inst)] + sp + regName[inst_t::getRs(inst)] + csp + std::to_string(inst_t::getImm16(inst));
}

/*
  000000 | N/A  | rt   | rd   | imm5 | 0000xx | shift-imm    0,2,3   SLL, SRL, SRA
  000000 | rs   | rt   | rd   | N/A  | 0001xx | shift-reg    4,6,7   SLLV, SRLV, SRAV
  000000 | rs   | N/A  | N/A  | N/A  | 001000 | jr           8
  000000 | rs   | N/A  | rd   | N/A  | 001001 | jalr         9
  000000 | <-----comment20bit------> | 00110x | sys/brk      12,13
  000000 | N/A  | N/A  | rd   | N/A  | 0100x0 | mfhi/mflo    16, 18
  000000 | rs   | N/A  | N/A  | N/A  | 0100x1 | mthi/mtlo    17, 19
  000000 | rs   | rt   | N/A  | N/A  | 0110xx | mul/div      24,25,26,27
  000000 | rs   | rt   | rd   | N/A  | 10xxxx | alu-reg      32,33,34,35,36,37,38,39,42,43
*/

std::string decodeAlu(uint32_t inst) {
    switch(inst_t::getFunc(inst)) {
        case 0: case 2: case 3: 
            if(inst == 0) return "nop";
            return func[inst_t::getFunc(inst)] + " meep";
        case 4: case 6: case 7:
            return func[inst_t::getFunc(inst)] + " meep";
        case 8: 
            return func[inst_t::getFunc(inst)] + " meep";
        case 9: 
            return func[inst_t::getFunc(inst)] + " meep";
        case 12: case 13:
            return func[inst_t::getFunc(inst)] + " meep";
        case 16: case 18:
            return func[inst_t::getFunc(inst)] + " meep";
        case 17: case 19:
            return func[inst_t::getFunc(inst)] + " meep";
        case 24: case 25: case 26: case 27:
            return func[inst_t::getFunc(inst)] + " meep";
        case 32: case 33: case 34: case 35: case 36: case 37: case 38: case 39: case 42: case 43:
            return func[inst_t::getFunc(inst)] + sp + regName[inst_t::getRd(inst)] + csp + regName[inst_t::getRs(inst)] + csp + regName[inst_t::getRt(inst)];
        default:
            return "N/A";
    }
    return func[inst_t::getFunc(inst)];
}

std::string decode(uint32_t inst) {
    auto oper = inst_t::getOp(inst);
    switch(oper) {
        case 0: return decodeAlu(inst); // ALU operations
        case 1: return decodeBranch(inst); // Branch operations
        case 2: case 3: // j/jal
            return opcode[oper] + sp + std::to_string(inst_t::getTarget(inst));
        case 4:
            return opcode[inst_t::getOp(inst)] + " meep";
        case 5:
            return opcode[inst_t::getOp(inst)] + " meep";
        case 6:
            return opcode[inst_t::getOp(inst)] + " meep";
        case 7:
            return opcode[inst_t::getOp(inst)] + " meep";
        case 8: case 9: case 10: case 11: case 12: case 13: case 14: // I-Type ALU ops
            return opcode[oper] + sp + regName[inst_t::getRt(inst)] + csp + regName[inst_t::getRs(inst)] + csp + std::to_string(inst_t::getImm16(inst));
        case 15: // lui
            return opcode[oper] + sp + regName[inst_t::getRt(inst)] + csp + std::to_string(inst_t::getImm16(inst));
        case 16:
            return opcode[inst_t::getOp(inst)] + " meep";
        case 17:
            return opcode[inst_t::getOp(inst)] + " meep";
        case 18:
            return opcode[inst_t::getOp(inst)] + " meep";
        case 19:
            return opcode[inst_t::getOp(inst)] + " meep";
        case 32: case 33: case 34: case 35: case 36: case 37: case 38: case 40: case 41: case 42: case 43: case 46: // load/store ops
            return opcode[oper] + sp + regName[inst_t::getRt(inst)] + csp + std::to_string(inst_t::getImm16(inst)) + op + regName[inst_t::getRs(inst)] + cp;
        case 48:
            return opcode[inst_t::getOp(inst)] + " meep";
        case 49:
            return opcode[inst_t::getOp(inst)] + " meep";
        case 50:
            return opcode[inst_t::getOp(inst)] + " meep";
        case 51:
            return opcode[inst_t::getOp(inst)] + " meep";
        case 56:
            return opcode[inst_t::getOp(inst)] + " meep";
        case 57:
            return opcode[inst_t::getOp(inst)] + " meep";
        case 58:
            return opcode[inst_t::getOp(inst)] + " meep";
        case 59:
            return opcode[inst_t::getOp(inst)] + " meep";
        default: return "N/A";
    }
}

int main(int argc, char *argv[]) {
    std::vector<std::string> args;
    for(int i=0;i<argc;i++) args.push_back(argv[i]);

    if(args.size() < 2) {
        std::cerr<<"Provide a file path as an argument.\n";
        return 1;
    }

    std::ifstream infile(args[1]);
    if(!infile.is_open()) {
        std::cerr<<"Couldn't open file at "<<args[1]<<'\n';
        return 1;
    }

    std::vector<uint32_t> data;
    infile.seekg(0,std::ios::end);
    size_t fileSize = infile.tellg();
    std::cout<<"Reading "<<fileSize<<" bytes from "<<args[1]<<'\n';
    data.resize(fileSize / 4 + fileSize % 4);
    infile.seekg(0,std::ios::beg);
    infile.read(reinterpret_cast<char*>(data.data()), fileSize);

    size_t offset = 0;
    for(auto d:data) {
        std::printf("%06x: %08x\t", offset, d);
        std::cout<<decode(d)<<'\n';
        offset += 4;
    }

}

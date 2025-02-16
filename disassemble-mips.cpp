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
    static uint32_t getImm16(uint32_t inst) { return inst & 0xffff; }
    static uint32_t getTarget(uint32_t inst) { return inst & 0x3f'fffff; }
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
    "zero", "at", "v0", "v1",
    "a0", "a1", "a2", "a3",
    "t0", "t1", "t2", "t3",
    "t4", "t5", "t6", "t7",
    "s0", "s1", "s2", "s3",
    "s4", "s5", "s6", "s7",
    "t8", "t9", "k0", "k1",
    "gp", "sp", "fp", "ra"
};

static const std::string opcode[] {
    "SPECIAL", "BcondZ", "J",    "JAL",   "BEQ",  "BNE", "BLEZ", "BGTZ",
    "ADDI",    "ADDIU",  "SLTI", "SLTIU", "ANDI", "ORI", "XORI", "LUI",
    "COP0",    "COP1",   "COP2", "COP3",  "N/A",  "N/A", "N/A",  "N/A",
    "N/A",     "N/A",    "N/A",  "N/A",   "N/A",  "N/A", "N/A",  "N/A",
    "LB",      "LH",     "LWL",  "LW",    "LBU",  "LHU", "LWR",  "N/A",
    "SB",      "SH",     "SWL",  "SW",    "N/A",  "N/A", "SWR",  "N/A",
    "LWC0",    "LWC1",   "LWC2", "LWC3",  "N/A",  "N/A", "N/A",  "N/A",
    "SWC0",    "SWC1",   "SWC2", "SWC3",  "N/A",  "N/A", "N/A",  "N/A"
};

// SPECIAL table
static const std::string func[] {
    "SLL",  "N/A",   "SRL",  "SRA",  "SLLV",    "N/A",   "SRLV", "SRAV",
    "JR",   "JALR",  "N/A",  "N/A",  "SYSCALL", "BREAK", "N/A",  "N/A",
    "MFHI", "MTHI",  "MFLO", "MTLO", "N/A",     "N/A",   "N/A",  "N/A",
    "MULT", "MULTU", "DIV",  "DIVU", "N/A",     "N/A",   "N/A",  "N/A",
    "ADD",  "ADDU",  "SUB",  "SUBU", "AND",     "OR",    "XOR",  "NOR",
    "N/A",  "N/A",   "SLT",  "SLTU", "N/A",     "N/A",   "N/A",  "N/A",
    "N/A",  "N/A",   "N/A",  "N/A",  "N/A",     "N/A",   "N/A",  "N/A"
    "N/A",  "N/A",   "N/A",  "N/A",  "N/A",     "N/A",   "N/A",  "N/A"
};

// BcondZ table
static const std::string cond[] {
    "BLTZ",   "BGEZ",   "BLTZ", "BGEZ", "BLTZ", "BGEZ", "BLTZ", "BGEZ", 
    "BLTZ",   "BGEZ",   "BLTZ", "BGEZ", "BLTZ", "BGEZ", "BLTZ", "BGEZ", 
    "BLTZAL", "BGEZAL", "BLTZ", "BGEZ", "BLTZ", "BGEZ", "BLTZ", "BGEZ", 
    "BLTZ",   "BGEZ",   "BLTZ", "BGEZ", "BLTZ", "BGEZ", "BLTZ", "BGEZ"
}

std::string decodeBranch(uint32_t inst) {
    switch(inst_t::getRt(inst)) {
    }
}


std::string decodeAlu(uint32_t inst) {
    switch(inst_t::getFunc(inst)) {
        case 0:
        case 2:
        case 3:
        case 4:
        case 6:
        case 7:
        case 8: 
        case 9: 
        case 12: 
        case 13:
        case 16: 
        case 17: 
        case 18: 
        case 19:
        case 24: 
        case 25: 
        case 26: 
        case 27:
        case 32: 
        case 33: 
        case 34: 
        case 35: 
        case 36: 
        case 37: 
        case 38: 
        case 39:
        case 42: 
        case 43:
        default:
            return "N/A"
    }
}

std::string decode(uint32_t inst) {
    switch(inst_t::getOp(inst)) {
        case 0: return decodeAlu(inst);
        case 1: return decodeBranch(inst);
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
        case 12:
        case 13:
        case 14:
        case 15:
        case 16:
        case 17:
        case 18:
        case 19:
        case 32:
        case 33:
        case 34:
        case 35:
        case 36:
        case 37:
        case 38:
        case 40:
        case 41:
        case 42:
        case 43:
        case 46:
        case 48:
        case 49:
        case 50:
        case 51:
        case 56:
        case 57:
        case 58:
        case 59:
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
    data.resize(fileSize);
    infile.seekg(0,std::ios::beg);
    infile.read(reinterpret_cast<char*>(data.data()), fileSize);

    size_t offset = 0;
    for(auto d:data) {
        std::printf("%06x: %08x\n", offset, d);
        offset += 4;
    }

}

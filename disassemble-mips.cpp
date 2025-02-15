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

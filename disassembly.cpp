#include<iostream>
#include<fstream>
#include<vector>
#include<string>
#include<cstdint>

#include "disassembly.hpp"
#include "instruction.hpp"

std::string decodeBranch(uint32_t inst) {
    return cond[inst_t::getRt(inst)] + sp + regName[inst_t::getRs(inst)] + csp + std::to_string(inst_t::getImm16(inst));
}

std::string decodeAlu(uint32_t inst) {
    switch(inst_t::getFunc(inst)) {
        case 0: case 2: case 3: // SLL SRL SRA
            if(inst == 0) return "nop";
            return func[inst_t::getFunc(inst)] + sp + regName[inst_t::getRd(inst)] + csp + regName[inst_t::getRt(inst)] + csp + std::to_string(inst_t::getImm5(inst));
        case 4: case 6: case 7: // SLLV SRLV SRAV
            return func[inst_t::getFunc(inst)] + sp + regName[inst_t::getRd(inst)] + csp + regName[inst_t::getRt(inst)] + csp + regName[inst_t::getRs(inst)];
        case 8: // JR
            return func[inst_t::getFunc(inst)] + sp + regName[inst_t::getRs(inst)];
        case 9: // JALR
            return func[inst_t::getFunc(inst)] + sp + regName[inst_t::getRd(inst)] + csp + regName[inst_t::getRs(inst)];
        case 12: case 13:
            return func[inst_t::getFunc(inst)] + sp + std::to_string(inst>>6); // top 6 bits are 0, next 20 bits are a "comment" value
        case 16: case 18:
            return func[inst_t::getFunc(inst)] + sp + regName[inst_t::getRd(inst)];
        case 17: case 19:
            return func[inst_t::getFunc(inst)] + sp + regName[inst_t::getRs(inst)];
        case 24: case 25: case 26: case 27:
            return func[inst_t::getFunc(inst)] + sp + regName[inst_t::getRs(inst)] + csp + regName[inst_t::getRt(inst)];
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

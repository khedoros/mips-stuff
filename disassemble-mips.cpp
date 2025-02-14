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

static const std::string formats[] {
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

ADD    rd, rs, rt     rd <-- rs + rt   000000 sssss ttttt ddddd 00000 100000   note: overflow exception if highest-order carry-out bits differ, and rd isn't modified
ADDU   rd, rs, rt     rd <-- rs + rt   000000 sssss ttttt ddddd 00000 100001



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

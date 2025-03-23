#pragma once

#include<string>
#include<cstdint>

#include "instruction.hpp"

enum format_t {
    reg,
    imm,
    jmp
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

static const std::string sp = " ";
static const std::string csp = ", ";
static const std::string op = "(";
static const std::string cp = ")";

std::string decode(uint32_t inst);
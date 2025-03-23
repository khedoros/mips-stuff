#pragma once

#include<cstdint>
#include<array>

class memmap {
    typedef uint32_t address;
    std::array<uint8_t, 2 * 1024 * 1024> ram;
    std::array<uint8_t, 1024> scratchPad;
    std::array<uint8_t, 512 * 1024> bios;
public:
    memmap();
    uint8_t read8(address addr);
    uint16_t read16(address addr);
    uint32_t read32(address addr);
    void write8(address addr, uint8_t val);
    void write16(address addr, uint8_t val);
    void write32(address addr, uint8_t val);
};
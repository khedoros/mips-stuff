#include "memmap.h"

/*
    KUSEG     KSEG0     KSEG1
  00000000h 80000000h A0000000h  2048K  Main RAM (first 64K reserved for BIOS)
  1F000000h 9F000000h BF000000h  8192K  Expansion Region 1 (ROM/RAM)
  1F800000h 9F800000h    --      1K     Scratchpad (D-Cache used as Fast RAM)
  1F801000h 9F801000h BF801000h  4K     I/O Ports
  1F802000h 9F802000h BF802000h  8K     Expansion Region 2 (I/O Ports)
  1FA00000h 9FA00000h BFA00000h  2048K  Expansion Region 3 (SRAM BIOS region for DTL cards)
  1FC00000h 9FC00000h BFC00000h  512K   BIOS ROM (Kernel) (4096K max)
        FFFE0000h (in KSEG2)     0.5K   Internal CPU control registers (Cache Control)

  Address   Name   i-Cache     Write-Queue
  00000000h KUSEG  Yes         Yes
  80000000h KSEG0  Yes         Yes
  A0000000h KSEG1  No          No
  C0000000h KSEG2  (No code)   No
*/

memmap::memmap() {}
uint8_t memmap::read8(address addr) {
    return 0;
}
uint16_t memmap::read16(address addr) {
    return 0;
}
uint32_t memmap::read32(address addr) {
    return 0;
}
void memmap::write8(address addr, uint8_t val) {}
void memmap::write16(address addr, uint8_t val) {}
void memmap::write32(address addr, uint8_t val) {}
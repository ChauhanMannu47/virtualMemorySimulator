#ifndef VIRTUAL_MEMORY_SYSTEM_H
#define VIRTUAL_MEMORY_SYSTEM_H

#include <vector>
#include <cstdint>

#include "constants.h"
#include "TLB.h"
#include "PhysicalMemory.h"
#include "ReplacementPolicy.h"

struct PageTableEntry {

    int frameNumber = -1;
    bool valid = false;
    bool referenced = false;
};

class VirtualMemorySystem {

    std::vector<PageTableEntry> pageTable;

    PhysicalMemory memory;
    TLB tlb;

    ReplacementPolicy *policy;

    int accesses = 0;
    int pageFaults = 0;
    int tlbHits = 0;

public:

    VirtualMemorySystem(ReplacementPolicy *p);

    void accessMemory(uint32_t virtualAddress);

    void printStats();
};

#endif
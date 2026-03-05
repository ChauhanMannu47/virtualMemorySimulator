#include "VirtualMemorySystem.h"
#include <iostream>
#include <iomanip>
#include <string>

using namespace std;

VirtualMemorySystem::VirtualMemorySystem(ReplacementPolicy *p) {

    policy = p;
    pageTable.resize(NUM_PAGES);
}

void VirtualMemorySystem::accessMemory(uint32_t virtualAddress) {

    accesses++;

    int page = virtualAddress >> OFFSET_BITS;
    int offset = virtualAddress & (PAGE_SIZE - 1);

    int frame = tlb.lookup(page);

    if(frame != -1) {

        tlbHits++;
        policy->notifyAccess(frame);
    }
    else {

        PageTableEntry &pte = pageTable[page];

        if(!pte.valid) {

            pageFaults++;

            int freeFrame = memory.findFreeFrame();

            if(freeFrame != -1) {

                frame = freeFrame;
            }
            else {

                frame = policy->chooseVictim(memory.frames);

                int victimPage = memory.frames[frame].pageNumber;

                pageTable[victimPage].valid = false;

                tlb.invalidatePage(victimPage);
            }

            memory.frames[frame].occupied = true;
            memory.frames[frame].pageNumber = page;

            pte.valid = true;
            pte.frameNumber = frame;

            policy->notifyInsert(frame);
        }

        frame = pte.frameNumber;

        tlb.insert(page, frame);
    }

    uint32_t physicalAddress =
        (frame << OFFSET_BITS) | offset;
}

void VirtualMemorySystem::printStats() {

    cout << "\nstatistics"<<endl;
    cout<<"policy-name: "<<policy->name<<endl;
    cout << "Accesses: " << accesses << endl;
    cout << "Page Faults: " << pageFaults << endl;
    cout << "TLB Hits: " << tlbHits << endl;

    cout << "Page Fault Rate: "
         << fixed << setprecision(2)
         << (double)pageFaults / accesses * 100
         << "%\n";

    cout << "TLB Hit Rate: "
         << (double)tlbHits / accesses * 100
         << "%\n";
}
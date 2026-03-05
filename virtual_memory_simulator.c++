#include <iostream>
#include <vector>
#include <queue>
#include <list>
#include <random>
#include <iomanip>

using namespace std;

//constants

const int PAGE_SIZE = 4096;
const int OFFSET_BITS = 12;

const int VIRTUAL_ADDRESS_BITS = 20;
const int NUM_PAGES = 1 << (VIRTUAL_ADDRESS_BITS - OFFSET_BITS);

const int NUM_FRAMES = 8;
const int TLB_SIZE = 8;

//PAGE TABLE ENTRY

struct PageTableEntry {
    int frameNumber = -1;
    bool valid = false;
    bool referenced = false;
};

//frame struct

struct Frame {
    int pageNumber = -1;
    bool occupied = false;
};

//struct describing TLB entry

struct TLBEntry {
    int pageNumber = -1;
    int frameNumber = -1;
    bool valid = false;
};

//page replacement policy

class ReplacementPolicy {
public:
    virtual int chooseVictim(vector<Frame>& frames) = 0;
    virtual void notifyAccess(int frame) {}
    virtual void notifyInsert(int frame) {}
    virtual ~ReplacementPolicy() {}
};

//FIFO

class FIFOReplacement : public ReplacementPolicy {

    queue<int> q;

public:

    void notifyInsert(int frame) override {
        q.push(frame);
    }

    int chooseVictim(vector<Frame>& frames) override {
        int victim = q.front();
        q.pop();
        q.push(victim);
        return victim;
    }
};

//LRU

class LRUReplacement : public ReplacementPolicy {

    list<int> order;

public:

    void notifyAccess(int frame) override {

        order.remove(frame);
        order.push_back(frame);
    }

    void notifyInsert(int frame) override {
        order.push_back(frame);
    }

    int chooseVictim(vector<Frame>& frames) override {

        int victim = order.front();
        order.pop_front();
        order.push_back(victim);
        return victim;
    }
};

//clock

class ClockReplacement : public ReplacementPolicy {

    vector<bool> refBits;
    int hand;

public:

    ClockReplacement() {

        refBits.resize(NUM_FRAMES,false);
        hand = 0;
    }

    void notifyAccess(int frame) override {

        refBits[frame] = true;
    }

    void notifyInsert(int frame) override {

        refBits[frame] = true;
    }

    int chooseVictim(vector<Frame>& frames) override {

        while(true) {

            if(!refBits[hand]) {

                int victim = hand;
                hand = (hand+1) % NUM_FRAMES;
                return victim;
            }

            refBits[hand] = false;
            hand = (hand+1) % NUM_FRAMES;
        }
    }
};

//TLB

class TLB {

    vector<TLBEntry> entries;
    int nextReplace = 0;

public:

    TLB() {
        entries.resize(TLB_SIZE);
    }

    int lookup(int page) {

        for(auto &e : entries) {

            if(e.valid && e.pageNumber == page)
                return e.frameNumber;
        }

        return -1;
    }

    void insert(int page,int frame) {

        entries[nextReplace].pageNumber = page;
        entries[nextReplace].frameNumber = frame;
        entries[nextReplace].valid = true;

        nextReplace = (nextReplace+1)%TLB_SIZE;
    }

    void invalidatePage(int page) {

        for(auto &e : entries) {

            if(e.valid && e.pageNumber == page)
                e.valid = false;
        }
    }
};

//physical memory

class PhysicalMemory {

public:

    vector<Frame> frames;

    PhysicalMemory() {
        frames.resize(NUM_FRAMES);
    }

    int findFreeFrame() {

        for(int i=0;i<NUM_FRAMES;i++)
            if(!frames[i].occupied)
                return i;

        return -1;
    }
};

//virtual memory system

class VirtualMemorySystem {

    vector<PageTableEntry> pageTable;
    PhysicalMemory memory;
    TLB tlb;

    ReplacementPolicy *policy;

    int accesses=0;
    int pageFaults=0;
    int tlbHits=0;

public:

    VirtualMemorySystem(ReplacementPolicy *p) {

        policy = p;
        pageTable.resize(NUM_PAGES);
    }

    void accessMemory(uint32_t virtualAddress) {

        accesses++;

        int page = virtualAddress >> OFFSET_BITS;
        int offset = virtualAddress & (PAGE_SIZE-1);

        // cout << "\nVA:0x" << hex << virtualAddress << dec;
        // cout << " Page:" << page;
        // cout << " Offset:" << offset << endl;
        int frame = tlb.lookup(page);
        if(frame!=-1) {
            tlbHits++;
            // cout<<"TLB HIT -> Frame "<<frame<<endl;
            policy->notifyAccess(frame);
        } else {
            // cout<<"TLB MISS\n";
            PageTableEntry &pte = pageTable[page];
            if(!pte.valid) {
                pageFaults++;
                // cout<<"PAGE FAULT\n";
                int freeFrame = memory.findFreeFrame();
                if(freeFrame!=-1) {

                    frame = freeFrame;

                } else {

                    frame = policy->chooseVictim(memory.frames);

                    int victimPage = memory.frames[frame].pageNumber;

                    pageTable[victimPage].valid = false;

                    tlb.invalidatePage(victimPage);
                }

                memory.frames[frame].occupied = true;
                memory.frames[frame].pageNumber = page;

                pte.valid=true;
                pte.frameNumber=frame;

                policy->notifyInsert(frame);
            }

            frame = pte.frameNumber;

            tlb.insert(page,frame);
        }

        uint32_t physicalAddress =
        (frame << OFFSET_BITS) | offset;

        // cout<<"Physical Address:0x"<<hex<<physicalAddress<<dec<<endl;
    }

    void printStats() {

        cout<<"\n======= STATS =======\n";

        cout<<"Accesses: "<<accesses<<endl;
        cout<<"Page Faults: "<<pageFaults<<endl;
        cout<<"TLB Hits: "<<tlbHits<<endl;

        cout<<"Page Fault Rate: "
            <<fixed<<setprecision(2)
            <<(double)pageFaults/accesses*100
            <<"%\n";

        cout<<"TLB Hit Rate: "
            <<(double)tlbHits/accesses*100
            <<"%\n";
    }
};

int main() {

    ReplacementPolicy *policy;

    cout<<"Choose Replacement Policy\n";
    cout<<"1 FIFO\n2 LRU\n3 CLOCK\n";

    int choice;
    cin>>choice;

    if(choice==1) policy = new FIFOReplacement();
    else if(choice==2) policy = new LRUReplacement();
    else policy = new ClockReplacement();

    VirtualMemorySystem vm(policy);

    random_device rd;
    mt19937 gen(rd());

    uniform_int_distribution<uint32_t> dist(0,(1<<20)-1);

    for(int i=0;i<1000;i++){
    uint32_t addr;

    if(rand()%100 < 70)
    {
        int page = rand()%8;
        int offset = rand()%PAGE_SIZE;
        addr = (page<<OFFSET_BITS) | offset;
    }
    else
    {
        addr = dist(gen);
    }

    vm.accessMemory(addr);
    }

    vm.printStats();

    delete policy;

    return 0;
}
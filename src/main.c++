#include <iostream>
#include <random>
using namespace std;

#include "VirtualMemorySystem.h"
#include "ReplacementPolicy.h"
#include "constants.h"



int main() {

    ReplacementPolicy *policy;

    cout<<"running tests";
    
    for(int i=1;i<=3;i++){
        if(i==1) policy = new FIFOReplacement();
        else if(i==2) policy = new LRUReplacement();
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


    }
    return 0;
}
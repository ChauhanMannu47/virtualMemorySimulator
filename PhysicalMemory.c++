#include "PhysicalMemory.h"

PhysicalMemory::PhysicalMemory() {

    frames.resize(NUM_FRAMES);
}

int PhysicalMemory::findFreeFrame() {

    for(int i = 0; i < NUM_FRAMES; i++)
        if(!frames[i].occupied)
            return i;

    return -1;
}
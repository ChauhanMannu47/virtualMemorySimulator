#ifndef PHYSICAL_MEMORY_H
#define PHYSICAL_MEMORY_H

#include <vector>
#include "constants.h"

struct Frame {

    int pageNumber = -1;
    bool occupied = false;
};

class PhysicalMemory {

public:

    std::vector<Frame> frames;

    PhysicalMemory();

    int findFreeFrame();
};

#endif
#ifndef TLB_H
#define TLB_H

#include <vector>
#include "constants.h"

struct TLBEntry {

    int pageNumber = -1;
    int frameNumber = -1;
    bool valid = false;
};

class TLB {

    std::vector<TLBEntry> entries;
    int nextReplace = 0;

public:

    TLB();

    int lookup(int page);

    void insert(int page,int frame);

    void invalidatePage(int page);
};

#endif
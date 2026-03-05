#include "TLB.h"

TLB::TLB() {
    entries.resize(TLB_SIZE);
}

int TLB::lookup(int page) {

    for(auto &e : entries) {

        if(e.valid && e.pageNumber == page)
            return e.frameNumber;
    }

    return -1;
}

void TLB::insert(int page,int frame) {

    entries[nextReplace].pageNumber = page;
    entries[nextReplace].frameNumber = frame;
    entries[nextReplace].valid = true;

    nextReplace = (nextReplace+1)%TLB_SIZE;
}

void TLB::invalidatePage(int page) {

    for(auto &e : entries) {

        if(e.valid && e.pageNumber == page)
            e.valid = false;
    }
}
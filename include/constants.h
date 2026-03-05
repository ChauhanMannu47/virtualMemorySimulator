#ifndef CONSTANTS_H
#define CONSTANTS_H

const int PAGE_SIZE = 4096;
const int OFFSET_BITS = 12;

const int VIRTUAL_ADDRESS_BITS = 20;
const int NUM_PAGES = 1 << (VIRTUAL_ADDRESS_BITS - OFFSET_BITS);

const int NUM_FRAMES = 8;
const int TLB_SIZE = 8;

#endif
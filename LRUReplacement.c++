#include "ReplacementPolicy.h"
#include <list>

static std::list<int> order;

void LRUReplacement::notifyAccess(int frame) {

    order.remove(frame);
    order.push_back(frame);
}

void LRUReplacement::notifyInsert(int frame) {

    order.push_back(frame);
}

int LRUReplacement::chooseVictim(std::vector<Frame>& frames) {

    int victim = order.front();
    order.pop_front();
    order.push_back(victim);

    return victim;
}
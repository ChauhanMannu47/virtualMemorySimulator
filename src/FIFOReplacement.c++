#include "ReplacementPolicy.h"
#include <queue>

static std::queue<int> q;

void FIFOReplacement::notifyInsert(int frame) {
    q.push(frame);
}

int FIFOReplacement::chooseVictim(std::vector<Frame>& frames) {

    int victim = q.front();
    q.pop();
    q.push(victim);

    return victim;
}
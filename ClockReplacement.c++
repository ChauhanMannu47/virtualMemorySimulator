#include "ReplacementPolicy.h"
#include "constants.h"
#include <vector>

static std::vector<bool> refBits(NUM_FRAMES,false);
static int hand = 0;

// ClockReplacement::ClockReplacement() {}

void ClockReplacement::notifyAccess(int frame) {

    refBits[frame] = true;
}

void ClockReplacement::notifyInsert(int frame) {

    refBits[frame] = true;
}

int ClockReplacement::chooseVictim(std::vector<Frame>& frames) {

    while(true) {

        if(!refBits[hand]) {

            int victim = hand;
            hand = (hand+1)%NUM_FRAMES;
            return victim;
        }

        refBits[hand] = false;
        hand = (hand+1)%NUM_FRAMES;
    }
}
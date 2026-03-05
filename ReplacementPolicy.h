#ifndef REPLACEMENT_POLICY_H
#define REPLACEMENT_POLICY_H

#include <vector>
#include<string>

struct Frame;

class ReplacementPolicy {
public:
    virtual int chooseVictim(std::vector<Frame>& frames) = 0;
    virtual void notifyAccess(int frame) {}
    virtual void notifyInsert(int frame) {}
    std::string name;
    virtual ~ReplacementPolicy() {}
};

class FIFOReplacement : public ReplacementPolicy {

public:
    FIFOReplacement() {
        name = "FIFO";
    }
    void notifyInsert(int frame) override;
    int chooseVictim(std::vector<Frame>& frames) override;
};

class LRUReplacement : public ReplacementPolicy {

public:
    LRUReplacement() {
        name = "LRU";
    }
    void notifyAccess(int frame) override;
    void notifyInsert(int frame) override;
    int chooseVictim(std::vector<Frame>& frames) override;
};

class ClockReplacement : public ReplacementPolicy {

public:
    ClockReplacement() {
        name = "CLOCK";
    }
    void notifyAccess(int frame) override;
    void notifyInsert(int frame) override;
    int chooseVictim(std::vector<Frame>& frames) override;
};

#endif
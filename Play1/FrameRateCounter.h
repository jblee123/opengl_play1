#pragma once

#include <queue>

class FrameRateCounter
{
public:
    FrameRateCounter(unsigned int histortyLength);
    virtual ~FrameRateCounter();

    virtual float incorportateTime(unsigned int currentTime);
    virtual float getFrameRate(unsigned int currentTime);

protected:
    unsigned int m_historyLength;
    std::queue<unsigned int> m_history;
};

#pragma once

#include <queue>

class FrameRateCounter
{
public:
    FrameRateCounter(unsigned int historty_length);
    virtual ~FrameRateCounter();

    virtual float incorportateTime(unsigned int current_time);
    virtual float getFrameRate(unsigned int current_time);

protected:
    unsigned int m_historyLength;
    std::queue<unsigned int> m_history;
};

#include "FrameRateCounter.h"

FrameRateCounter::FrameRateCounter(unsigned int histortyLength) :
    m_historyLength(histortyLength) {
}

FrameRateCounter::~FrameRateCounter() {
}

float FrameRateCounter::incorportateTime(unsigned int currentTime) {
    m_history.push(currentTime);
    return getFrameRate(currentTime);
}

float FrameRateCounter::getFrameRate(unsigned int currentTime) {
    while ((currentTime - m_history.front()) >= m_historyLength) {
        m_history.pop();
    }
    return m_history.size() ?
        ((float)m_history.size() / ((float)m_historyLength / 1000.0f)) : 0;
}

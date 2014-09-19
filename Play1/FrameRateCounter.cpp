#include "FrameRateCounter.h"

FrameRateCounter::FrameRateCounter(unsigned int histortyLength) :
    m_historyLength(histortyLength) {
}

FrameRateCounter::~FrameRateCounter() {
}

float FrameRateCounter::incorportateTime(unsigned int current_time) {
    m_history.push(current_time);
    return getFrameRate(current_time);
}

float FrameRateCounter::getFrameRate(unsigned int current_time) {
    while ((current_time - m_history.front()) >= m_historyLength) {
        m_history.pop();
    }
    return m_history.size() ?
        ((float)m_history.size() / ((float)m_historyLength / 1000.0f)) : 0;
}

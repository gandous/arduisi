#include "Clock.h"

Clock::Clock(): _time(0)
{}

Clock::~Clock()
{}

void Clock::restart()
{
    _time = millis();
}

uint64_t Clock::getElapsedTime() const
{
    return (millis() - _time);
}

uint64_t Clock::getElapsedTimeAsSecond() const
{
    return ((millis() - _time) / 1000);
}

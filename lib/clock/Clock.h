#pragma once

#include <Arduino.h>

class Clock {
    public:
        Clock();
        ~Clock();

        void restart();
        uint64_t getElapsedTime() const;
    protected:
    private:
        uint64_t _time;
};
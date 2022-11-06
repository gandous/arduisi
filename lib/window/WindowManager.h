#pragma once

#include <Adafruit_NeoMatrix.h>
#include "Window.h"
#include "Time.h"

static const uint8_t WINDOW_NB = 1;

class WindowManager {
    public:
        WindowManager();
        ~WindowManager();

        void init();
        void update(Adafruit_NeoMatrix &matrix);
    protected:
    private:
        window::Window *_windows[WINDOW_NB] = {&_time};
        uint8_t _current;
        window::Time _time;
};
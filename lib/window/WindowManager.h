#pragma once

#include <Adafruit_NeoMatrix.h>
#include <Clock.h>
#include "Window.h"
#include "Time.h"
#include "Weather.h"

static const uint8_t WINDOW_NB = 2;

class WindowManager {
    public:
        WindowManager();
        ~WindowManager();

        void init();
        void update(Adafruit_NeoMatrix &matrix);
    protected:
    private:
        enum WindowState {
            TRANSITION,
            FIX,
        };
        void update_display(Adafruit_NeoMatrix &matrix);

        window::Window *_windows[WINDOW_NB] = {&_time, &_weather};
        uint8_t _current_win;
        uint8_t _prev_win;
        Clock _switch_window_clock;
        Clock _update_clock;
        int _x;
        int _y;
        WindowState _state;
        window::Time _time;
        window::Weather _weather;
};
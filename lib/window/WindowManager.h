#pragma once

#include <Matrix.h>
#include <Clock.h>
#include "Window.h"
#include "Time.h"
#include "Weather.h"
#include "Heater.h"

static const uint8_t WINDOW_NB = 3;

class WindowManager {
    public:
        enum WindowState {
            TRANSITION,
            FIX,
        };

        WindowManager();
        ~WindowManager();

        void init();
        void update(Matrix &matrix);
        void update_data(Matrix &matrix);
        WindowState get_state() const;
    protected:
    private:
        void update_display(Matrix &matrix);

        window::Window *_windows[WINDOW_NB] = {&_time, &_weather, &_heater};
        uint8_t _current_win;
        uint8_t _prev_win;
        Clock _switch_window_clock;
        Clock _update_clock;
        int _x;
        int _y;
        WindowState _state;
        window::Time _time;
        window::Weather _weather;
        window::Heater _heater;
};
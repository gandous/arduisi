#pragma once

#include <Matrix.h>
#include <Clock.h>
#include "Window.h"
#include "Time.h"
#include "Weather.h"
#include "HeaterOutsideTemp.h"
#include "HeaterInsideTemp.h"

static const uint8_t WINDOW_NB = 4;

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

        window::Window *_windows[WINDOW_NB] = {&_time, &_weather, &_heater_inside_temp, &_heater_outside_temp};
        uint8_t _current_win;
        uint8_t _prev_win;
        Clock _switch_window_clock;
        Clock _update_clock;
        int _x;
        int _y;
        WindowState _state;
        window::Time _time;
        window::Weather _weather;
        window::HeaterInsideTemp _heater_inside_temp;
        window::HeaterOutsideTemp _heater_outside_temp;
};
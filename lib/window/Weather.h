#pragma once

#include <Adafruit_NeoMatrix.h>
#include <Clock.h>
#include "Window.h"

static const int METEO_UPDATE_INTERVAL = 3600 * 2; // 2hours

namespace window {

class Weather: public Window {
    public:
        Weather();
        ~Weather();

        void init();
        void update(Adafruit_NeoMatrix &matrix, int x, int y);
    protected:
    private:
        void update_data();

        Clock _update_clock;
        char _icon[4];
        int _feels_like;
};

}
#pragma once

#include <Matrix.h>
#include <Clock.h>
#include "Window.h"

static const int METEO_UPDATE_INTERVAL = 3600 * 2; // 2hours

namespace window {

class Weather: public Window {
    public:
        Weather();
        ~Weather();

        void init();
        void update_data();
        void update(Matrix &matrix, int x, int y);
    protected:
    private:
        enum Icon {
            CLEAR_SKY_D,
            CLEAR_SKY_N,
            FOG_D,
            MAINLY_CLEAR_D,
            MAINLY_CLEAR_N,
            OVERCAST_D,
            PARTLY_CLOUDY_D,
            RAIN_MEDIUM_D,
            RAIN_SLIGHT_D,
            RAIN_SLIGHT_N,
            SNOW_D,
            THUNDERSTORM_D,
            NONE,
        };
        void parse_icon(int icon);
        void draw_icon(Matrix &matrix, int x, int y);

        Clock _update_clock;
        Icon _icon;
        int _precipitation_sum;
};

}
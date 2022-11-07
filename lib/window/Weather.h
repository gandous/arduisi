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
        void update(Matrix &matrix, int x, int y);
    protected:
    private:
        enum Icon {
            I01D,
            I01N,
            I02D,
            I02N,
            I03D,
            I03N,
            I04D,
            I04N,
            I09D,
            I09N,
            I10D,
            I10N,
            I11D,
            I11N,
            I13D,
            I13N,
            I50D,
            I50N,
            NONE,
        };
        void update_data();
        void parse_icon(const char *icon);
        void draw_icon(Matrix &matrix, int x, int y);

        Clock _update_clock;
        Icon _icon;
        int _feels_like;
};

}
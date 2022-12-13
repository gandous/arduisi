#pragma once

#include <WiFiUdp.h>
#include <NTPClient.h>
#include <Matrix.h>
#include "Window.h"

namespace window {

static const int HEATER_FETCH_ERROR = 99;

class Heater: public Window {
    public:
        Heater();
        ~Heater();

        void init();
        void update_data();
    protected:
        void draw_temperature(Matrix &matrix, int x, int y, const uint16_t img[], int temp);
        int extract_int(char *buffer, size_t value_idx);
        virtual void parse_data(char *data) = 0;
    private:
};

}
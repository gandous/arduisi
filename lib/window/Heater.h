#pragma once

#include <WiFiUdp.h>
#include <NTPClient.h>
#include <Matrix.h>
#include "Window.h"

namespace window {

class Heater: public Window {
    public:
        Heater();
        ~Heater();

        void init();
        void update(Matrix &matrix, int x, int y);
    protected:
    private:
        void update_data();
        int _temp;
};

}
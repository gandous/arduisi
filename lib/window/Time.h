#pragma once

#include <WiFiUdp.h>
#include <NTPClient.h>
#include <Adafruit_NeoMatrix.h>
#include "Window.h"

namespace window {

class Time: public Window {
    public:
        Time();
        ~Time();

        void init();
        void update(Adafruit_NeoMatrix &matrix, int x, int y);
    protected:
    private:
        WiFiUDP _ntp_udp;
        NTPClient _time_client;
};

}
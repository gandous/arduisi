#pragma once

#include <Arduino.h>
#include <Adafruit_NeoMatrix.h>

namespace window {

class Window {
    public:
        virtual ~Window() {};

        virtual void init() = 0;
        virtual void update(Adafruit_NeoMatrix &matrix, int x, int y) = 0;
    protected:
    private:
};

}

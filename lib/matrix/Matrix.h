#pragma once

#include <Arduino.h>
#include <Adafruit_NeoMatrix.h>

class Matrix: public Adafruit_NeoMatrix {
    public:
        Matrix(uint8_t matrixW, uint8_t matrixH, uint8_t tX, uint8_t tY, uint8_t pin, uint8_t matrixType, neoPixelType ledType);
        ~Matrix();

        void drawRGB(int x, int y, const uint16_t data[], uint8_t w, uint8_t h);
    protected:
    private:
};

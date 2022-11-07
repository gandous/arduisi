#include "Matrix.h"

Matrix::Matrix(uint8_t matrixW, uint8_t matrixH, uint8_t tX, uint8_t tY, uint8_t pin, uint8_t matrixType, neoPixelType ledType):
    Adafruit_NeoMatrix(matrixW, matrixH, tX, tY, pin, matrixType, ledType)
{}

Matrix::~Matrix()
{}

void Matrix::drawRGB(int x, int y, const uint16_t data[], uint8_t w, uint8_t h)
{
    startWrite();
    for (uint8_t img_y = 0; img_y < h; img_y++) {
        for (uint8_t img_x = 0; img_x < w; img_x++) {
            writePixel(x + img_x, y + img_y, pgm_read_word(&data[img_x + img_y * w]));
        }
    }
    endWrite();
}
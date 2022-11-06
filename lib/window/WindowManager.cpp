#include "WindowManager.h"

WindowManager::WindowManager(): _current(0)
{}

WindowManager::~WindowManager()
{}

void WindowManager::init()
{
    for (uint8_t i = 0; i < WINDOW_NB; i++)
        _windows[i]->init();
}

void WindowManager::update(Adafruit_NeoMatrix &matrix)
{
    _windows[_current]->update(matrix);
}

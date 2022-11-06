#include "WindowManager.h"

WindowManager::WindowManager(): _current_win(0), _prev_win(0), _x(0), _y(0), _state(WindowState::FIX)
{}

WindowManager::~WindowManager()
{}

void WindowManager::init()
{
    for (uint8_t i = 0; i < WINDOW_NB; i++)
        _windows[i]->init();
    _switch_window_clock.restart();
}

void WindowManager::update(Adafruit_NeoMatrix &matrix)
{
    matrix.clear();
    if (_switch_window_clock.getElapsedTime() > 10000) {
        _switch_window_clock.restart();
        _prev_win = _current_win;
        _current_win++;
        if (_current_win >= WINDOW_NB)
            _current_win = 0;
        _state = WindowState::TRANSITION;
        _y = matrix.height();
    }
    if (_update_clock.getElapsedTime() > 100) {
        _update_clock.restart();
        update_display(matrix);
    }
}


void WindowManager::update_display(Adafruit_NeoMatrix &matrix)
{
    switch (_state) {
        case WindowState::TRANSITION:
            _y--;
            _windows[_prev_win]->update(matrix, _x, _y - matrix.height());
            _windows[_current_win]->update(matrix, _x, _y);
            if (_y == 0)
                _state = WindowState::FIX;
            break;
        case WindowState::FIX:
            _windows[_current_win]->update(matrix, 0, 0);
            break;
    }
}
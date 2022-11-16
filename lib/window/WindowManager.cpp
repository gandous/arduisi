#include "WindowManager.h"

WindowManager::WindowManager(): _init(false), _current_win(0), _prev_win(0), _x(0), _y(0), _state(WindowState::FIX)
{}

WindowManager::~WindowManager()
{}

void WindowManager::init()
{
    _init = true;
    for (uint8_t i = 0; i < WINDOW_NB; i++)
        _windows[i]->init();
    _switch_window_clock.restart();
}

void WindowManager::update(Matrix &matrix)
{
    if (!_init)
        init();
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
        matrix.clear();
        update_display(matrix);
        matrix.show();
    }
}

WindowManager::WindowState WindowManager::get_state() const
{
    return (_state);
}

void WindowManager::update_display(Matrix &matrix)
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
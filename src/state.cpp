#include "state.h"

State::State(): _state(States::NONE)
{}

State::~State()
{}

void State::set(States state) {
    _state = (States)(_state | state);
}

void State::remove(States state) {
    _state = (States)(_state ^ state);
}

bool State::has(States state) {
    return ((_state & state) == state);
}

State state;
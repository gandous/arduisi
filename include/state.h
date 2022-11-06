#pragma once

enum State {
    CONNECTED,
    FAILED_TO_CONNECT,
    NONE,
};

static State state = State::NONE;
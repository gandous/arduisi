#pragma once

class State {
    public:
        State();
        ~State();

        enum States {
            CONNECTED = 1,
            FAILED_TO_CONNECT = 1 << 1,
            SCREEN_OFF = 1 << 2,
            NONE = 0,
        };

        void set(States state);
        void remove(States state);
        bool has(States state);
    protected:
    private:
        States _state;
};

extern State state;
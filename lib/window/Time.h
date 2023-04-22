#pragma once

#include <Matrix.h>
#include <Arduino.h>
#include <ezTime.h>
#include "Window.h"

namespace window {

class Time: public Window {
    public:
        Time();
        ~Time();

        void init();
        void update_data();
        void update(Matrix &matrix, int x, int y);
    protected:
    private:
        Timezone _tz;
};

}
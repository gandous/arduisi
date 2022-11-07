#pragma once

#include <Arduino.h>
#include <Matrix.h>

namespace window {

class Window {
    public:
        virtual ~Window() {};

        virtual void init() = 0;
        virtual void update(Matrix &matrix, int x, int y) = 0;
    protected:
    private:
};

}

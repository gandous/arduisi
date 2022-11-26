#pragma once

#include <Arduino.h>
#include <Matrix.h>

namespace window {

class Window {
    public:
        virtual ~Window() {};

        enum UpdateStatus {
            FAILED = 0,
            SUCCESS = 1,
        };

        virtual void init() = 0;
        virtual void update_data() = 0;
        virtual void update(Matrix &matrix, int x, int y) = 0;
        virtual UpdateStatus get_update_status() const
        {
            return (_status);
        };
    protected:
        UpdateStatus _status = FAILED;
    private:
};

}

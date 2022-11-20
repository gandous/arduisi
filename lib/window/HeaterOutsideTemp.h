#pragma once

#include "Heater.h"

namespace window {

class HeaterOutsideTemp: public Heater {
    public:
        HeaterOutsideTemp();
        ~HeaterOutsideTemp();

        void update(Matrix &matrix, int x, int y);
    protected:
        void parse_data(char *data);
    private:
        int _temp;
};

}
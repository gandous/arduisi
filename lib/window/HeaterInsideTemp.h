#pragma once

#include "Heater.h"

namespace window {

class HeaterInsideTemp: public Heater {
    public:
        HeaterInsideTemp();
        ~HeaterInsideTemp();

        void update(Matrix &matrix, int x, int y);
    protected:
        void parse_data(char *data);
    private:
        int _temp;
};

}
#include "HeaterIcon.h"
#include "HeaterOutsideTemp.h"

namespace window {

HeaterOutsideTemp::HeaterOutsideTemp(): _temp(99)
{}

HeaterOutsideTemp::~HeaterOutsideTemp()
{}

void HeaterOutsideTemp::update(Matrix &matrix, int x, int y)
{
    draw_temperature(matrix, x, y, IMG_HOUSE_TEMP_OUTSIDE, _temp);
}

void HeaterOutsideTemp::parse_data(char *data)
{
    int new_temp = extract_int(data, 18);

    if (new_temp != HEATER_FETCH_ERROR)
        _temp = new_temp;
}

}
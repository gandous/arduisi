#include "HeaterIcon.h"
#include "HeaterInsideTemp.h"

namespace window {

HeaterInsideTemp::HeaterInsideTemp(): _temp(99)
{}

HeaterInsideTemp::~HeaterInsideTemp()
{}

void HeaterInsideTemp::update(Matrix &matrix, int x, int y)
{
    draw_temperature(matrix, x, y, IMG_HOUSE_TEMP_INSIDE, _temp);
}

void HeaterInsideTemp::parse_data(char *data)
{
    _temp = extract_int(data, 60);
}

}
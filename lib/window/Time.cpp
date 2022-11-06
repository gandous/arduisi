#include "Time.h"

namespace window {

Time::Time(): _time_client(_ntp_udp, "europe.pool.ntp.org", 3600)
{}

Time::~Time()
{}

void Time::init()
{
    _time_client.begin();
}

void Time::update(Adafruit_NeoMatrix &matrix, int x, int y)
{
    _time_client.update();
    matrix.drawRect(11 + x, 1 + y, 2, 2, matrix.Color(0, 0, 255));
    matrix.drawRect(11 + x, 4 + y, 2, 2, matrix.Color(0, 0, 255));
    matrix.setCursor(1 + x, y);
    matrix.printf("%02d", _time_client.getHours());
    matrix.setCursor(14 + x, y);
    matrix.printf("%02d", _time_client.getMinutes());
    matrix.show();
}

}
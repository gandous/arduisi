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

void Time::update(Adafruit_NeoMatrix &matrix)
{
    _time_client.update();
    matrix.clear();
    matrix.drawRect(11, 1, 2, 2, matrix.Color(0, 0, 255));
    matrix.drawRect(11, 4, 2, 2, matrix.Color(0, 0, 255));
    matrix.setCursor(1, 0);
    matrix.printf("%02d", _time_client.getHours());
    matrix.setCursor(14, 0);
    matrix.printf("%02d", _time_client.getMinutes());
    matrix.show();
}

}
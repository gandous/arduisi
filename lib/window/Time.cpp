#include "Time.h"

namespace window {

Time::Time()
{}

Time::~Time()
{}

void Time::init()
{
    // setDebug(ezDebugLevel_t::INFO);
    _tz.setLocation("Europe/Paris");
    setServer("europe.pool.ntp.org");
    _status = waitForSync() ? UpdateStatus::SUCCESS : UpdateStatus::FAILED;
}

void Time::update_data()
{
    updateNTP();
    _status = waitForSync() ? UpdateStatus::SUCCESS : UpdateStatus::FAILED;
}

void Time::update(Matrix &matrix, int x, int y)
{
    time_t date = now();
    matrix.drawRect(11 + x, 1 + y, 2, 2, matrix.Color(0, 0, 255));
    matrix.drawRect(11 + x, 4 + y, 2, 2, matrix.Color(0, 0, 255));
    matrix.setCursor(1 + x, y);
    matrix.printf("%02d", _tz.hour(date, ezLocalOrUTC_t::UTC_TIME));
    matrix.setCursor(14 + x, y);
    matrix.printf("%02d", _tz.minute(date, ezLocalOrUTC_t::UTC_TIME));
}

}
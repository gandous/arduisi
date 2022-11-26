#include <ESP8266HTTPClient.h>
#include <ArduinoJson.hpp>
#include "Weather.h"
#include "WeatherConfig.h"
#include "WeatherIcon.h"

namespace window {

Weather::Weather(): _icon(Icon::CLEAR_SKY_D), _precipitation_sum(99)
{}

Weather::~Weather()
{}

void Weather::init()
{
    update_data();
}

void Weather::update_data()
{
    WiFiClient client;
    HTTPClient http;
    ArduinoJson::StaticJsonDocument<1024> doc;
    if (http.begin(client, WEATHER_API_URL)) {
        Serial.print("[HTTP] GET...\n");
        int httpCode = http.GET();
        if (httpCode > 0) {
            Serial.printf("[HTTP] GET... code: %d\n", httpCode);

            if (httpCode == HTTP_CODE_OK) {
                ArduinoJson::DeserializationError err = deserializeJson(doc, http.getString());
                if (err != ArduinoJson::DeserializationError::Ok) {
                    Serial.println(err.c_str());
                    _status = UpdateStatus::FAILED;
                    return;
                }

                parse_icon(doc["daily"]["weathercode"][0].as<int>());
                _precipitation_sum = round(doc["daily"]["precipitation_sum"][0].as<float>());
                _status = UpdateStatus::SUCCESS;
            }
        } else {
                Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
                _status = UpdateStatus::FAILED;
        }

      http.end();
    } else {
      _status = UpdateStatus::FAILED;
      Serial.printf("[HTTP] Unable to connect\n");
    }
}

void Weather::update(Matrix &matrix, int x, int y)
{
    if (_update_clock.getElapsedTimeAsSecond() > METEO_UPDATE_INTERVAL) {
        Serial.println(_update_clock.getElapsedTimeAsSecond());
        _update_clock.restart();
        update_data();
    }
    matrix.setCursor(x + 9, y);
    matrix.printf("%2dm", _precipitation_sum);
    draw_icon(matrix, x, y);
}

void Weather::parse_icon(int icon)
{
    switch (icon) {
        case 0:
            _icon = Icon::CLEAR_SKY_D;
            break;
        case 1:
            _icon = Icon::MAINLY_CLEAR_D;
            break;
        case 2:
            _icon = Icon::PARTLY_CLOUDY_D;
            break;
        case 3:
            _icon = Icon::OVERCAST_D;
            break;
        case 45:
        case 48:
            _icon = Icon::FOG_D;
            break;
        case 51:
        case 56:
        case 61:
        case 66:
        case 80:
            _icon = Icon::RAIN_SLIGHT_D;
            break;
        case 53:
        case 55:
        case 57:
        case 63:
        case 67:
        case 77:
        case 81:
        case 82:
            _icon = Icon::RAIN_MEDIUM_D;
            break;
        case 71:
        case 73:
        case 75:
        case 85:
        case 86:
            _icon = Icon::SNOW_D;
            break;
        case 95:
        case 96:
        case 99:
            _icon = Icon::THUNDERSTORM_D;
            break;
        default:
            break;
    }
}

void Weather::draw_icon(Matrix &matrix, int x, int y)
{
    switch (_icon) {
        case Icon::CLEAR_SKY_D:
            matrix.drawRGB(x, y, IMG_CLEAR_SKY_D, IMG_CLEAR_SKY_D_W, IMG_CLEAR_SKY_D_H);
            break;
        case Icon::CLEAR_SKY_N:
            matrix.drawRGB(x, y, IMG_CLEAR_SKY_N, IMG_CLEAR_SKY_N_W, IMG_CLEAR_SKY_N_H);
            break;
        case Icon::FOG_D:
            matrix.drawRGB(x, y, IMG_FOG_D, IMG_FOG_D_W, IMG_FOG_D_H);
            break;
        case Icon::MAINLY_CLEAR_D:
            matrix.drawRGB(x, y, IMG_MAINLY_CLEAR_D, IMG_MAINLY_CLEAR_D_W, IMG_MAINLY_CLEAR_D_H);
            break;
        case Icon::MAINLY_CLEAR_N:
            matrix.drawRGB(x, y, IMG_MAINLY_CLEAR_N, IMG_MAINLY_CLEAR_N_W, IMG_MAINLY_CLEAR_N_H);
            break;
        case Icon::OVERCAST_D:
            matrix.drawRGB(x, y, IMG_OVERCAST_D, IMG_OVERCAST_D_W, IMG_OVERCAST_D_H);
            break;
       case Icon::PARTLY_CLOUDY_D:
            matrix.drawRGB(x, y, IMG_PARTLY_CLOUDY_D, IMG_PARTLY_CLOUDY_D_W, IMG_PARTLY_CLOUDY_D_H);
            break;
       case Icon::RAIN_MEDIUM_D:
            matrix.drawRGB(x, y, IMG_RAIN_MEDIUM_D, IMG_RAIN_MEDIUM_D_W, IMG_RAIN_MEDIUM_D_H);
            break;
       case Icon::RAIN_SLIGHT_D:
            matrix.drawRGB(x, y, IMG_RAIN_SLIGHT_D, IMG_RAIN_SLIGHT_D_W, IMG_RAIN_SLIGHT_D_H);
            break;
        case Icon::RAIN_SLIGHT_N:
            matrix.drawRGB(x, y, IMG_RAIN_SLIGHT_N, IMG_RAIN_SLIGHT_N_W, IMG_RAIN_SLIGHT_N_H);
            break;
       case Icon::SNOW_D:
            matrix.drawRGB(x, y, IMG_SNOW_D, IMG_SNOW_D_W, IMG_SNOW_D_H);
            break;
       case Icon::THUNDERSTORM_D:
            matrix.drawRGB(x, y, IMG_THUNDERSTORM_D, IMG_THUNDERSTORM_D_W, IMG_THUNDERSTORM_D_H);
            break;
        default:
            break;
    }
}

}
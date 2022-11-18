#include <ESP8266HTTPClient.h>
#include <ArduinoJson.hpp>
#include "Weather.h"
#include "WeatherKey.h"
#include "WeatherIcon.h"

namespace window {

Weather::Weather(): _icon(Icon::I11D), _feels_like(99)
{}

Weather::~Weather()
{}

void Weather::init()
{
    update_data();
}

void Weather::update(Matrix &matrix, int x, int y)
{
    if (_update_clock.getElapsedTimeAsSecond() > METEO_UPDATE_INTERVAL) {
        Serial.println(_update_clock.getElapsedTimeAsSecond());
        _update_clock.restart();
        update_data();
    }
    matrix.setCursor(x + 9, y);
    matrix.printf("%2dc", _feels_like);
    matrix.drawPixel(x + 19, y + 0, matrix.Color(0, 0, 255));
    draw_icon(matrix, x, y);
}

void Weather::update_data()
{
    WiFiClient client;
    HTTPClient http;
    ArduinoJson::StaticJsonDocument<512> doc;

    if (http.begin(client, "http://api.openweathermap.org/data/2.5/weather?appid=" + WEATHER_APIKEY + "&q=chaville&units=metric")) {  // HTTP
        Serial.print("[HTTP] GET...\n");
        int httpCode = http.GET();
        if (httpCode > 0) {
            Serial.printf("[HTTP] GET... code: %d\n", httpCode);

            if (httpCode == HTTP_CODE_OK) {
                deserializeJson(doc, http.getString());

                parse_icon(doc["weather"][0]["icon"].as<const char*>());
                _feels_like = round(doc["main"]["feels_like"].as<float>());
            }
        } else {
                Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
        }

      http.end();
    } else {
      Serial.printf("[HTTP} Unable to connect\n");
    }
}

void Weather::parse_icon(const char *icon)
{
    if (strncmp(icon, "01", 2) == 0) {
        _icon = Icon::I01D;
    } else if (strncmp(icon, "02", 2) == 0) {
        _icon = Icon::I02D;
    } else if (strncmp(icon, "03", 2) == 0) {
        _icon = Icon::I03D;
    } else if (strncmp(icon, "04", 2) == 0) {
        _icon = Icon::I04D;
    } else if (strncmp(icon, "09", 2) == 0) {
        _icon = Icon::I09D;
    } else if (strncmp(icon, "10", 2) == 0) {
        _icon = Icon::I10D;
    } else if (strncmp(icon, "11", 2) == 0) {
        _icon = Icon::I11D;
    } else if (strncmp(icon, "13", 2) == 0) {
        _icon = Icon::I13D;
    } else if (strncmp(icon, "50", 2) == 0) {
        _icon = Icon::I50D;
    }
    if (icon[2] == 'n')
        _icon = (Icon)(_icon + 1);
}

void Weather::draw_icon(Matrix &matrix, int x, int y)
{
    switch (_icon) {
        case Icon::I01D:
            matrix.drawRGB(x, y, IMG_01D, IMG_01D_W, IMG_01D_H);
            break;
        case Icon::I01N:
            matrix.drawRGB(x, y, IMG_01N, IMG_01N_W, IMG_01N_H);
            break;
        case Icon::I02D:
            matrix.drawRGB(x, y, IMG_02D, IMG_02D_W, IMG_02D_H);
            break;
        case Icon::I02N:
            matrix.drawRGB(x, y, IMG_02N, IMG_02N_W, IMG_02N_H);
            break;
        case Icon::I03D:
        case Icon::I03N:
            matrix.drawRGB(x, y, IMG_03D, IMG_03D_W, IMG_03D_H);
            break;
        case Icon::I04D:
        case Icon::I04N:
            matrix.drawRGB(x, y, IMG_04D, IMG_04D_W, IMG_04D_H);
            break;
        case Icon::I09D:
        case Icon::I09N:
            matrix.drawRGB(x, y, IMG_09D, IMG_09D_W, IMG_09D_H);
            break;
        case Icon::I10D:
            matrix.drawRGB(x, y, IMG_10D, IMG_10D_W, IMG_10D_H);
            break;
        case Icon::I10N:
            matrix.drawRGB(x, y, IMG_10N, IMG_10N_W, IMG_10N_H);
            break;
        case Icon::I11D:
        case Icon::I11N:
            matrix.drawRGB(x, y, IMG_11D, IMG_11D_W, IMG_11D_H);
            break;
        case Icon::I13D:
        case Icon::I13N:
            matrix.drawRGB(x, y, IMG_13D, IMG_13D_W, IMG_13D_H);
            break;
        case Icon::I50D:
        case Icon::I50N:
            matrix.drawRGB(x, y, IMG_50D, IMG_50D_W, IMG_50D_H);
            break;
        default:
            break;
    }
}

}
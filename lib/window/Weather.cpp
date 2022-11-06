#include <ESP8266HTTPClient.h>
#include <ArduinoJson.hpp>
#include "Weather.h"
#include "WeatherKey.h"

namespace window {

Weather::Weather(): _feels_like(99)
{}

Weather::~Weather()
{}

void Weather::init()
{
    // update_data();
}

void Weather::update(Adafruit_NeoMatrix &matrix, int x, int y)
{
    if (_update_clock.getElapsedTimeAsSecond() > METEO_UPDATE_INTERVAL) {
        Serial.println(_update_clock.getElapsedTimeAsSecond());
        _update_clock.restart();
        update_data();
    }
    matrix.setCursor(x + 9, y);
    matrix.printf("%2dc", _feels_like);
    matrix.drawPixel(x + 23, y + 0, matrix.Color(0, 0, 255));
    matrix.show();
}

void Weather::update_data()
{
    WiFiClient client;
    HTTPClient http;
    ArduinoJson::StaticJsonDocument<512> doc;

    if (http.begin(client, "http://api.openweathermap.org/data/2.5/weather?appid=" + WEATHER_APIKEY + "&q=paris&units=metric")) {  // HTTP
        Serial.print("[HTTP] GET...\n");
        int httpCode = http.GET();
        if (httpCode > 0) {
            Serial.printf("[HTTP] GET... code: %d\n", httpCode);

            if (httpCode == HTTP_CODE_OK) {
                deserializeJson(doc, http.getString());
                strncpy(_icon, doc["weather"][0]["icon"].as<const char*>(), 4);
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

}
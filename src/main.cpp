#include <Adafruit_GFX.h>
#include <Matrix.h>
#include <Adafruit_NeoPixel.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <Uri.h>
#include <WindowManager.h>
#include <Clock.h>
#include "config.h"
#include "font.h"
#include "state.h"

static const int NETWORK_CONNECT_TRY = 50;


Matrix matrix = Matrix(8, 8, 3, 1, PIN_MATRIX,
  NEO_MATRIX_BOTTOM + NEO_MATRIX_LEFT + NEO_MATRIX_COLUMNS + NEO_MATRIX_PROGRESSIVE +
  NEO_TILE_LEFT,
  NEO_GRB + NEO_KHZ800);
WindowManager window_manager;
Clock update_data_clock;

void update_data(Matrix &matrix)
{
    // WiFi.mode(WIFI_STA);
    // WiFi.begin(SSID, PASSWORD);

    // matrix.drawPixel(0, matrix.height() - 1, matrix.Color(255, 113, 52));
    // matrix.show();
    // while (WiFi.status() != WL_CONNECTED) {
    //     delay(200);
    // }
    // matrix.drawPixel(0, matrix.height() - 1, matrix.Color(0, 255, 20));
    // matrix.show();
    // window_manager.update_data(matrix);
    // delay(3000);
    // WiFi.disconnect(true);
}

void setup()
{
    Serial.begin(115200);
    while (!Serial) continue;
    Serial.println();
    Serial.println("Starting...");

    matrix.begin();
    matrix.setTextWrap(false);
    matrix.setBrightness(20);
    matrix.setTextColor(matrix.Color(0, 0, 255));
    matrix.setFont(&font);


    WiFi.persistent(false);
    WiFi.mode(WIFI_STA);

    WiFi.begin(SSID, PASSWORD);
    uint16_t x = matrix.width() / 2;
    int8_t dir = 1;
    while (WiFi.status() != WL_CONNECTED) {
        matrix.clear();
        matrix.drawFastHLine(x, 5, 5, matrix.Color(255, 113, 52));
        matrix.show();
        x += dir;
        if (x > matrix.width() - 10 || x < 5)
            dir = -dir;
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    state.set(State::CONNECTED);
    window_manager.init();
    WiFi.disconnect(true);
}

void loop() {
    window_manager.update(matrix);
    if (update_data_clock.getElapsedTimeAsSecond() > DATA_UPDATE_INTERVAL) {
        update_data(matrix);
        update_data_clock.restart();
    }
}

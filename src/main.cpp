// Adafruit_NeoMatrix example for tiled NeoPixel matrices.  Scrolls
// 'Howdy' across three 10x8 NeoPixel grids that were created using
// NeoPixel 60 LEDs per meter flex strip.

#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Uri.h>
#include <Clock.h>
#include <eeprom.h>
#include <WindowManager.h>
#include "api.h"
#include "font.h"
#include "website.h"
#include "state.h"

static const uint8_t PIN_MATRIX = 5;
static const char *default_ssid = "Arduisi";
static const char *default_password = "arduisi0";


Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(8, 8, 3, 1, PIN_MATRIX,
  NEO_MATRIX_BOTTOM + NEO_MATRIX_LEFT + NEO_MATRIX_COLUMNS + NEO_MATRIX_PROGRESSIVE +
  NEO_TILE_LEFT,
  NEO_GRB + NEO_KHZ800);
ESP8266WebServer server(80);
Clock dclock;
WindowManager window_manager;

void setup_default_network() {
    Serial.println("Starting wifi hoststop");
    WiFi.softAP(default_ssid, default_password);
    Serial.print("IP: ");
    Serial.println(WiFi.softAPIP());
}

void setup()
{
    Serial.begin(115200);
    while (!Serial) continue;
    Serial.println();
    Serial.println("Starting...");

    EEPROM.begin(sizeof(eeprom_data_t));

    matrix.begin();
    matrix.setTextWrap(false);
    matrix.setBrightness(20);
    matrix.setTextColor(matrix.Color(0, 0, 255));
    matrix.setFont(&font);


    WiFi.persistent(false);
    WiFi.mode(WIFI_AP_STA);

    WiFi.begin(get_wifi_ssid(), get_wifi_password());
    uint16_t x = matrix.width() / 2;
    int8_t dir = 1;
    uint8_t try_nb = 0;
    while (WiFi.status() != WL_CONNECTED && try_nb < 50) {
        matrix.fillScreen(0);
        matrix.drawFastHLine(x, 5, 5, matrix.Color(255, 113, 52));
        matrix.show();
        x += dir;
        if (x > matrix.width() - 10 || x < 5)
            dir = -dir;
        delay(500);
        Serial.print(".");
    }
    if (try_nb >= 50) {
        setup_default_network();
        state = State::FAILED_TO_CONNECT;
    } else {
        Serial.println("");
        Serial.println("WiFi connected");
        Serial.println("IP address: ");
        Serial.println(WiFi.localIP());
        window_manager.init();
        state = State::CONNECTED;
    }

    setup_api(server);
    setup_preact(server);
    server.enableCORS(true);
    server.begin();
}

void loop() {
    if (dclock.getElapsedTime() > 700 && state == State::CONNECTED) {
        dclock.restart();
        window_manager.update(matrix);
    }
    server.handleClient();
}

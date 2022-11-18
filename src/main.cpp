#include <Adafruit_GFX.h>
#include <Matrix.h>
#include <Adafruit_NeoPixel.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <Uri.h>
#include <eeprom.h>
#include <WindowManager.h>
#include "arduisi.h"
#include "font.h"
#include "state.h"

static const uint8_t PIN_MATRIX = 5;
static const char *default_ssid = "Arduisi";
static const char *default_password = "arduisi0";
static const int NETWORK_CONNECT_TRY = 50;


Matrix matrix = Matrix(8, 8, 3, 1, PIN_MATRIX,
  NEO_MATRIX_BOTTOM + NEO_MATRIX_LEFT + NEO_MATRIX_COLUMNS + NEO_MATRIX_PROGRESSIVE +
  NEO_TILE_LEFT,
  NEO_GRB + NEO_KHZ800);
ESP8266WebServer server(80);
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
    while (WiFi.status() != WL_CONNECTED && try_nb < NETWORK_CONNECT_TRY) {
        try_nb++;
        matrix.fillScreen(0);
        matrix.drawFastHLine(x, 5, 5, matrix.Color(255, 113, 52));
        matrix.show();
        x += dir;
        if (x > matrix.width() - 10 || x < 5)
            dir = -dir;
        delay(500);
        Serial.print(".");
    }

    if (try_nb >= NETWORK_CONNECT_TRY) {
        WiFi.disconnect();
        setup_default_network();
        state.set(State::FAILED_TO_CONNECT);
        start_web_server(server);
    } else {
        Serial.println("");
        Serial.println("WiFi connected");
        Serial.println("IP address: ");
        Serial.println(WiFi.localIP());
        start_web_server(server);
        start_mdns();
        state.set(State::CONNECTED);
    }
}

void loop() {
    if (state.has(State::CONNECTED)) {
        if (!state.has(State::SCREEN_OFF)) {
            window_manager.update(matrix);
        } else {
            matrix.clear();
            matrix.show();
        }
        // Disable mdns update during transition so it doesnt cause framedrop
        if (window_manager.get_state() != WindowManager::TRANSITION)
            MDNS.update();
    } else if (state.has(State::FAILED_TO_CONNECT)) {
        show_ip(matrix);
    }
    server.handleClient();
}

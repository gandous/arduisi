#include <ESP8266mDNS.h>
#include <ESP8266WiFiAP.h>
#include <Clock.h>
#include <Matrix.h>
#include "state.h"
#include "website.h"
#include "api.h"
#include "arduisi.h"

void start_web_server(ESP8266WebServer &server)
{
    setup_api(server);
    setup_preact(server);
    server.enableCORS(true);
    server.begin();
}

void start_mdns()
{
    if (!MDNS.begin("arduisi")) {
        Serial.println("Failed to start mdns");
    }
    MDNS.addService("http", "tcp", 80);
}

static void display_ip(Matrix &matrix)
{
    static int16_t x_pos = 0;
    int16_t x;
    int16_t y;
    uint16_t w;
    uint16_t h;

    matrix.getTextBounds(WiFi.softAPIP().toString(), 0, 0, &x, &y, &w, &h);
    if (x_pos < -(int16_t)w) {
        x_pos = matrix.width();
    }
    matrix.setCursor(x_pos, 0);
    x_pos--;
    matrix.clear();
    matrix.print(WiFi.softAPIP());
    matrix.show();
}

void show_ip(Matrix &matrix)
{
    static Clock c;

    if (c.getElapsedTime() > 300) {
        c.restart();
        display_ip(matrix);
    }
}

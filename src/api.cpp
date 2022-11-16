#include <ArduinoJson.hpp>
#include "api.h"
#include "callback.h"
#include "eeprom.h"
#include "state.h"
#include "arduisi.h"

static const int API_STATUS_BUFFER_SIZE = 256;
static const int WIFI_CONNECT_TRY = 40;

void api_scan_wifi(ESP8266WebServer &server)
{
    String ssid;
    int32_t rssi;
    uint8_t encryptionType;
    uint8_t* bssid;
    int32_t channel;
    bool hidden;
    int scanResult;
    ArduinoJson::StaticJsonDocument<256> output;
    char sout[256];
    ArduinoJson::JsonArray networks = output.createNestedArray("networks");

    scanResult = WiFi.scanNetworks(false, false);

    for (int8_t i = 0; i < scanResult; i++) {
        WiFi.getNetworkInfo(i, ssid, encryptionType, rssi, bssid, channel, hidden);
        networks.add(ssid);
        delay(0);
    }

    size_t s = serializeJsonPretty(output, sout);
    server.send(200, "application/json", sout, s);
}

void api_connect_wifi(ESP8266WebServer &server)
{
    if (server.hasArg("plain") == false){ //Check if body received
        server.send(400, "text/plain", "Body not received");
        return;
    }
    ArduinoJson::StaticJsonDocument<200> doc;
    ArduinoJson::DeserializationError error = deserializeJson(doc, server.arg("plain"));
    if (error) {
        server.send(400, "text/plain", "Invalid body");
    }

    WiFi.begin(doc["ssid"].as<const char*>(), doc["password"].as<const char*>());
    Serial.print("Connecting to wifi");
    char count = 0;
    while (WiFi.status() != WL_CONNECTED && count < WIFI_CONNECT_TRY) {
        delay(500);
        count++;
        Serial.print(".");
    }
    if (count == WIFI_CONNECT_TRY) {
        Serial.println("Failed to connect");
        server.send(400);
    } else {
        Serial.println("\nConnected");
        server.send(200);
        for (uint8_t i = 0; i < 200; i++)// Let the client get the response before writing eeprom
            server.handleClient();
        Serial.println("Saving WIFI to eeprom");
        set_wifi_ssid(doc["ssid"].as<const char*>());
        set_wifi_password(doc["password"].as<const char*>());
        EEPROM.commit();
        Serial.println("WIFI saved");
        WiFi.softAPdisconnect();
        state.set(State::CONNECTED);
        state.remove(State::FAILED_TO_CONNECT);
        start_mdns();
        ESP.restart();
    }
}

void api_screen_off(ESP8266WebServer &server)
{
    state.set(State::SCREEN_OFF);
    server.send(200);
    Serial.printf("Off %d\n", state.has(State::SCREEN_OFF));
}

void api_screen_on(ESP8266WebServer &server)
{
    state.remove(State::SCREEN_OFF);
    server.send(200);
    Serial.printf("On %d\n", state.has(State::SCREEN_OFF));
}

void api_status(ESP8266WebServer &server)
{
    ArduinoJson::StaticJsonDocument<200> doc;
    char buffer[API_STATUS_BUFFER_SIZE];
    size_t size;

    doc["connected"] = state.has(State::CONNECTED);
    doc["failedToConnect"] = state.has(State::FAILED_TO_CONNECT);
    doc["screenOff"] = state.has(State::SCREEN_OFF);
    size = ArduinoJson::serializeJson(doc, buffer, API_STATUS_BUFFER_SIZE - 1);
    buffer[size] = '\0';
    server.send(200, "application/json", (const char*)buffer, size);
}

void api_send_cors_header(ESP8266WebServer &server)
{
    server.sendHeader(F("Access-Control-Max-Age"), F("600"));
    server.sendHeader(F("Access-Control-Allow-Methods"), F("PUT,POST,GET,OPTIONS"));
    server.sendHeader(F("Access-Control-Allow-Headers"), F("*"));
    server.send(204);
}

void setup_api(ESP8266WebServer &server) {
    server.on("/api/network", HTTPMethod::HTTP_GET, callback(server, api_scan_wifi));
    server.on("/api/network", HTTPMethod::HTTP_POST, callback(server, api_connect_wifi));
    server.on("/api/network", HTTPMethod::HTTP_OPTIONS, callback(server, api_send_cors_header));
    server.on("/api/screen-on", HTTPMethod::HTTP_POST, callback(server, api_screen_on));
    server.on("/api/screen-off", HTTPMethod::HTTP_POST, callback(server, api_screen_off));
    server.on("/api/status", HTTPMethod::HTTP_GET, callback(server, api_status));
    server.on("/api/status", HTTPMethod::HTTP_OPTIONS, callback(server, api_send_cors_header));
}
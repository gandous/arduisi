#include <ArduinoJson.hpp>
#include "api.h"
#include "callback.h"
#include "eeprom.h"

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
    while (WiFi.status() != WL_CONNECTED && count < 90) {
        delay(500);
        Serial.print(".");
    }
    if (count == 90) {
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
    }
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
}
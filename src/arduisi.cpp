#include <ESP8266mDNS.h>
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
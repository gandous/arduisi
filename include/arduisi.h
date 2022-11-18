#pragma once

#include <ESP8266WebServer.h>
#include <WindowManager.h>

void start_web_server(ESP8266WebServer &server);
void start_mdns();
void show_ip(Matrix &matrix);
